#include "tiny/etokenkind.h"

#include <string_view>
#include <unordered_map>

using std::string_view;
using std::unordered_map;

namespace {
using namespace tiny;
static unordered_map<ETokenKind, string_view>* s_pTokenToLiteral;
} // namespace

namespace tiny {

std::ostream& operator<<(std::ostream& os, ETokenKind token) {
  return os<<(*s_pTokenToLiteral)[token];
}

void initialze_etokenkind() {
#define TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(name) { ETokenKind::name, #name }

  s_pTokenToLiteral = new unordered_map<ETokenKind, string_view> {
    // keywords
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kIf),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kThen),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kElse),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kEnd),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kRepeat),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kUntil),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kRead),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kWrite),
  
    // operators
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kPlus),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kMinus),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kMultiply),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kDivide),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kEqual),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kLessThan),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kLeftBrace),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kRightBrace),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kSemicolon),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kAssign),

    // literal and identifier
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kNumber),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kIdentifier),

    // special
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kEOS),
    TINY_ETOKENKIND_CC_MAKE_HASH_ITEM(kError),
};

#undef TINY_ETOKENKIND_CC_MAKE_HASH_ITEM
}

void finalize_etokenkind() {
  delete s_pTokenToLiteral;
  s_pTokenToLiteral = nullptr;
}

}  // namespace tiny