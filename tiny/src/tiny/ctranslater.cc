#include "tiny/ctranslater.h"

#include <cassert>
#include <cstdlib>
#include <array>
#include <string_view>

#include "tiny/cast.h"
#include "tiny/castnodeexp.h"
#include "tiny/castnodeexpid.h"
#include "tiny/castnodeexpnum.h"
#include "tiny/castnodeexpop.h"
#include "tiny/castnoderoot.h"
#include "tiny/castnodestmt.h"
#include "tiny/castnodestmtif.h"
#include "tiny/castnodestmtrepeat.h"
#include "tiny/castnodestmtassign.h"
#include "tiny/castnodestmtread.h"
#include "tiny/castnodestmtwrite.h"
#include "tiny/cvmlabel.h"
#include "tiny/cvmopbuffer.h"
#include "tiny/cvmopbufferbuilder.h"
#include "tiny/eastnodekind.h"
#include "tiny/etokenkind.h"
#include "windy/basetype.h"

#include "windy/agility.h"

using std::atoi;
using std::array;
using std::unique_ptr;
using std::string_view;
using windy::i32;

namespace {
// todo: there are no string_view version stoi in std lib
//    maybe we should make one by ourselves ?
WINDY_AGILITY_FUTURE_REFACTOR();
i32 sv_to_i(string_view sv) {
  constexpr i32 kBufSize = 16;
  
  assert(sv.size()<kBufSize);
  
  array<char, kBufSize> buf {};
  const int sv_size = sv.size();
  sv.copy(buf.data(), sv_size);
  buf[sv_size] = '\0';
  
  return atoi(buf.data());
}
} // namespace

namespace tiny {

struct CTranslater::SImpl {
  static void translate(CAST& ast, CVMOpBufferBuilder& builder) {
    auto stmt_seq = static_cast<CASTNodeStmt*>(ast.entry());
    translateStmtSeq(*stmt_seq, builder);
  }

  static void translateStmtSeq(CASTNodeStmt& stmt_seq,
                               CVMOpBufferBuilder& builder) {
    for (IASTNode* sibling = &stmt_seq;
         sibling;
         sibling = sibling->sibling()) {
      auto stmt = static_cast<CASTNodeStmt*>(sibling);
      translateStmt(*stmt, builder);
    }
  }

  static void translateStmt(CASTNodeStmt& stmt, CVMOpBufferBuilder& builder) {
    EASTNodeKind kind = stmt.kind();

    switch (kind) {
      case EASTNodeKind::kStmtIf: {
        auto& stmt_if = static_cast<CASTNodeStmtIf&>(stmt);
        translateStmtIf(stmt_if, builder);
      }
      break;

      case EASTNodeKind::kStmtRepeat: {
        auto& stmt_repeat = static_cast<CASTNodeStmtRepeat&>(stmt);
        translateStmtRepeat(stmt_repeat, builder);
      }
      break;

      case EASTNodeKind::kStmtAssign: {
        auto& stmt_assign = static_cast<CASTNodeStmtAssign&>(stmt);
        translateStmtAssign(stmt_assign, builder);
      }
      break;

      case EASTNodeKind::kStmtRead: {
        auto& stmt_read = static_cast<CASTNodeStmtRead&>(stmt);
        translateStmtRead(stmt_read, builder);
      }
      break;

      case EASTNodeKind::kStmtWrite: {
        auto& stmt_write = static_cast<CASTNodeStmtWrite&>(stmt);
        translateStmtWrite(stmt_write, builder);
      }
      break;

      default:
        assert(!"Unknown stmt kind!");
    }
  }

  static void translateStmtIf(CASTNodeStmtIf& stmt,
                              CVMOpBufferBuilder& builder) {
    CVMLabel l_else = builder.newLabel();
    CVMLabel l_end = builder.newLabel();

    translateExp(*stmt.ifPart(), builder);
    builder.addOpJumpIfZero(l_else);

    translateStmtSeq(*stmt.thenPart(), builder);

    builder.addLabel(l_else);
    CASTNodeStmt* else_part = stmt.elsePart();
    if (else_part) {
      builder.addOpJump(l_end);
      translateStmtSeq(*else_part, builder);
    }
    builder.addLabel(l_end);
  }

  static void translateStmtRepeat(CASTNodeStmtRepeat& stmt,
                                  CVMOpBufferBuilder& builder) {
    CVMLabel l_repeat = builder.newLabel();

    builder.addLabel(l_repeat);
    translateStmtSeq(*stmt.repeatPart(), builder);

    translateExp(*stmt.untilPart(), builder);
    builder.addOpJumpIfZero(l_repeat);
  }

  static void translateStmtAssign(CASTNodeStmtAssign& stmt,
                                  CVMOpBufferBuilder& builder) {
    translateExp(*stmt.rightPart(), builder);
    string_view name = stmt.leftPart()->name();
    builder.addOpLookupOrNew(name);
    builder.addOpVariableWrite();
  }
  static void translateStmtRead(CASTNodeStmtRead& stmt,
                                CVMOpBufferBuilder& builder) {
    WINDY_AGILITY_NOT_FINISHED();

    assert(!"Not implemented yet!");
  }

  static void translateStmtWrite(CASTNodeStmtWrite& stmt,
                                 CVMOpBufferBuilder& builder) {
    string_view name = stmt.value();
    builder.addOpWrite(name);
  }

  static void translateExp(CASTNodeExp& exp, 
                           CVMOpBufferBuilder& builder) {
    EASTNodeKind kind = exp.kind();

    switch (kind) {
      case EASTNodeKind::kExpID:
        translateExpID(static_cast<CASTNodeExpID&>(exp), builder);
      break;

      case EASTNodeKind::kExpNum:
        translateExpNum(static_cast<CASTNodeExpNum&>(exp), builder);
      break;

      case EASTNodeKind::kExpOp:
        translateExpOp(static_cast<CASTNodeExpOp&>(exp), builder);
      break;

      default:
        assert(!"Unknown exp kind!");
    }
  }

  static void translateExpID(CASTNodeExpID& id,
                             CVMOpBufferBuilder& builder) {
    string_view name = id.value();
    builder.addOpLookup(name);
    builder.addOpVariableRead();
  }

  static void translateExpNum(CASTNodeExpNum& num, 
                              CVMOpBufferBuilder& builder) {
    i32 value = sv_to_i(num.value());
    builder.addOpPush(value);
  }

  static void translateExpOp(CASTNodeExpOp& exp_op, 
                             CVMOpBufferBuilder& builder) {
    ETokenKind token = exp_op.opToken();
    translateExp(*exp_op.leftPart(), builder);
    translateExp(*exp_op.rightPart(), builder);

    switch (token) {
      case ETokenKind::kEqual:
        builder.addOpEqual();
      break;

      case ETokenKind::kLessThan:
        builder.addOpLessThan();
      break;

      case ETokenKind::kPlus:
        builder.addOpPlus();
      break;

      case ETokenKind::kMinus:
        builder.addOpMinus();
      break;

      case ETokenKind::kMultiply:
        builder.addOpMultiply();
      break;

      case ETokenKind::kDivide:
        builder.addOpDivide();
      break;

      default:
        assert(!"Unknown op token!");
    }
  }
};

std::unique_ptr<CVMOpBuffer> CTranslater::translate(CAST& ast) {
  //CVMOpBuffer buf {};
  //CVMOpBufferBuilder builder {buf};
  CVMOpBufferBuilder builder {};
  builder.start();
  SImpl::translate(ast, builder);
  return builder.complete();

  //return buf;
}

}  // namespace tiny