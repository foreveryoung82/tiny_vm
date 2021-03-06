#pragma once
#include <memory>
#include <string_view>

#include "windy/basetype.h"
#include "tiny/etokenkind.h"
#include "tiny/stokenvalue.h"

namespace tiny {
class CSymbolTable;
} // namespace tiny

namespace tiny {

// ref class
// lexer for tiny vm
// precondition: src.size()>=1; src.last()=='\0'
class CLexer final {
 public:
  explicit CLexer(std::string_view src) noexcept;
  ~CLexer();

  ETokenKind next();
  // following must be called after a successful call to next() to have valid
  // return value
  ETokenKind token() const { return _token; }
  // literal value for kNumber and kIdentifier; undefined for others
  const STokenValue& value() const { return _value; }

 private:
  std::string_view _source;
  std::unique_ptr<CSymbolTable> _pSymbolTable;
  ETokenKind _token;
  STokenValue _value;
  windy::i32 _tokenStart;
  //windy::i32 _lineIdx;
  windy::i32 _nextCharIdx;
};

} // namespace tiny
