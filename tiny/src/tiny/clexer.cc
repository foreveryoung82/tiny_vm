#include "tiny/clexer.h"

#include <cassert>
#include <locale>
#include <memory>
#include <string_view>
#include <unordered_map>

#include <gsl/gsl>
#include "tiny/csymboltable.h"
#include "tiny/ssymboltableentry.h"
#include "tiny/etokenkind.h"
#include "tiny/stokenvalue.h"
#include "windy/basetype.h"

using std::isspace;
using std::isdigit;
using std::isalpha;
using std::ispunct;
using std::string_view;
using std::unique_ptr;
using std::unordered_map;
using gsl::index;
using gsl::narrow_cast;
using windy::i32;

namespace tiny {

CLexer::CLexer(std::string_view src) noexcept
    : _source(src),
      _pSymbolTable{new CSymbolTable{}},
      _token{ETokenKind::kError},
      _value{STokenValue{}},
      _tokenStart{0},
      //_lineIdx{0},
      _nextCharIdx{0} {
  assert(src.size()>=1);
  assert(src.back()=='\0');

  // keyword
  _pSymbolTable->insert("if", {ETokenKind::kIf});
  _pSymbolTable->insert("then", {ETokenKind::kThen});
  _pSymbolTable->insert("else", {ETokenKind::kElse});
  _pSymbolTable->insert("end", {ETokenKind::kEnd});
  _pSymbolTable->insert("repeat", {ETokenKind::kRepeat});
  _pSymbolTable->insert("until", {ETokenKind::kUntil});
  _pSymbolTable->insert("read", {ETokenKind::kRead});
  _pSymbolTable->insert("write", {ETokenKind::kWrite});

  // operator
  _pSymbolTable->insert("+", {ETokenKind::kPlus});
  _pSymbolTable->insert("-", {ETokenKind::kMinus});
  _pSymbolTable->insert("*", {ETokenKind::kMultiply});
  _pSymbolTable->insert("/", {ETokenKind::kDivide});
  _pSymbolTable->insert("=", {ETokenKind::kEqual});
  _pSymbolTable->insert("<", {ETokenKind::kLessThan});
  _pSymbolTable->insert("{", {ETokenKind::kLeftBrace});
  _pSymbolTable->insert("}", {ETokenKind::kRightBrace});
  _pSymbolTable->insert(";", {ETokenKind::kSemicolon});
  _pSymbolTable->insert(":=", {ETokenKind::kAssign});
}

CLexer::~CLexer() = default;

ETokenKind CLexer::next() {
  i32 state = 0;
  const index size = narrow_cast<index>(_source.size());
 
  for (index i=_nextCharIdx;i<size;++i) {
    char c = _source[i];
    constexpr char eos = '\0';

    switch (state) {
      case 0:
        if (isspace(c)) continue;
        else if (c == '\n') continue;
        else if (c == eos) {
          _tokenStart = i;
          _nextCharIdx = i+1;
          _token = ETokenKind::kEOS;
          
          return _token;
        } else if (isdigit(c)) {
          _tokenStart = i;
          state = 2;
          
          continue;
        } else if (isalpha(c)) {
          _tokenStart = i;
          state = 4;

          continue;
        } else if (ispunct(c)) {
          _tokenStart = i;
          state = 5;

          continue;
        } else {
          _tokenStart = i;
          _nextCharIdx = i+1;
          _token = ETokenKind::kError;

          return _token;
        }
      break;

      case 2:
        if (isdigit(c)) continue;
        else {
          _nextCharIdx = i;
          _token = ETokenKind::kNumber;
          string_view name(&_source[0]+_tokenStart, i-_tokenStart);
          _value.setLexeme(name);

          return _token;
        }
      break;

      case 4:
        if (isalpha(c)) continue;
        else {
          _nextCharIdx = i;
          string_view name(&_source[0]+_tokenStart, i-_tokenStart);
          SSymbolTableEntry* entry = _pSymbolTable->lookup(name);

          if (entry) {
            _token = entry->token;
 
            return _token;
          } else {
            _token = ETokenKind::kIdentifier;
            _value.setLexeme(name);

            return _token;
          }
        }
      break;

      case 5:
        if (ispunct(c)) continue;
        else {
          _nextCharIdx = i;
          string_view name(&_source[0]+_tokenStart, i-_tokenStart);
          SSymbolTableEntry* entry = _pSymbolTable->lookup(name);

          if (entry) {
            _token = entry->token;
 
            return _token;
          } else {
            _token = ETokenKind::kError;

            return _token;
          }          
        }
      break;
    }
  }

  return ETokenKind::kEOS; 
}

}  // namespace tiny