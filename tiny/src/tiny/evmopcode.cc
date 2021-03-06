#include "tiny/evmopcode.h"

#include <string_view>
#include <unordered_map>

using std::string_view;
using std::unordered_map;

namespace {
using namespace tiny;

static unordered_map<EVMOpCode, string_view>* s_pOpCodeToLiteral;
}

namespace tiny {

std::ostream& operator<<(std::ostream& os, EVMOpCode code) {
  return os<<(*s_pOpCodeToLiteral)[code];
}

void initialze_evmopcode() {
#define TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(name) { EVMOpCode::name, #name }

  s_pOpCodeToLiteral = new unordered_map<EVMOpCode, string_view> {
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kNOP),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kHalt),

      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kJump),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kJumpIfNotZero),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kJumpIfZero),

      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kLookup),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kLookupOrNew),

      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kVariableRead),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kVariableWrite),

      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kPush),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kPop),

      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kPlus),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kMinus),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kMultiply),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kDivide),

      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kEqual),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kLessThan),

      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kRead),
      TINY_EVMOPCODE_CC_MAKE_HASH_ITEM(kWrite),
  };

#undef TINY_EVMOPCODE_CC_MAKE_HASH_ITEM
}

void finalize_evmopcode() {
  delete s_pOpCodeToLiteral;
  s_pOpCodeToLiteral = nullptr;
}

}  // namespace tiny