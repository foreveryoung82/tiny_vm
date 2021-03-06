#include "tiny/eastnodekind.h"

#include <ostream>
#include <string_view>
#include <unordered_map>

using std::ostream;
using std::string_view;
using std::unordered_map;

namespace {
using namespace tiny;

static unordered_map<EASTNodeKind, string_view>* s_pKindToLiteral;
}

namespace tiny {

std::ostream& operator<<(std::ostream& os, EASTNodeKind kind) {
  return os<<(*s_pKindToLiteral)[kind];
}

void initialze_eastnodekind() {
#define TINY_EASTNODEKIND_CC_MAKE_ITEM(name) \
  {EASTNodeKind::name, #name}

  s_pKindToLiteral = new unordered_map<EASTNodeKind, string_view>{
      TINY_EASTNODEKIND_CC_MAKE_ITEM(kRoot),

      TINY_EASTNODEKIND_CC_MAKE_ITEM(kStmtIf),
      TINY_EASTNODEKIND_CC_MAKE_ITEM(kStmtRepeat),
      TINY_EASTNODEKIND_CC_MAKE_ITEM(kStmtAssign),
      TINY_EASTNODEKIND_CC_MAKE_ITEM(kStmtRead),
      TINY_EASTNODEKIND_CC_MAKE_ITEM(kStmtWrite),

      TINY_EASTNODEKIND_CC_MAKE_ITEM(kExpOp),
      TINY_EASTNODEKIND_CC_MAKE_ITEM(kExpID),
      TINY_EASTNODEKIND_CC_MAKE_ITEM(kExpNum),
  };

#undef TINY_EASTNODEKIND_CC_MAKE_ITEM
}

void finalize_eastnodekind() {
  delete s_pKindToLiteral;
  s_pKindToLiteral = nullptr;
}

}  // namespace tiny