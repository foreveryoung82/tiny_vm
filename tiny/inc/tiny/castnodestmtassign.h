#pragma once
#include "tiny/castnodestmt.h"
#include "tiny/castnodeexpid.h"

namespace tiny {

class CASTNodeStmtAssign: public CASTNodeStmt {
 private:
  CASTNodeStmtAssign() noexcept : CASTNodeStmt{EASTNodeKind::kStmtAssign}{}

  static constexpr int kLeftPartIdx = 0;
  static constexpr int kRightPartIdx = 1;

 public:
  virtual ~CASTNodeStmtAssign()=default;

  void setLeftPart(CASTNodeExpID* id) { _children[kLeftPartIdx] = id; }
  CASTNodeExpID* leftPart() {
    return static_cast<CASTNodeExpID*>(_children[kLeftPartIdx]);
  }

  void setRightPart(CASTNodeExp* exp) { _children[kRightPartIdx] = exp; }
  CASTNodeExp* rightPart() {
    return static_cast<CASTNodeExp*>(_children[kRightPartIdx]);
  }

 private:
  friend class CAST;
};

} // namespace tiny#pragma once
