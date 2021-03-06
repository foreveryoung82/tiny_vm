#pragma once
#include "tiny/castnodestmt.h"
#include "tiny/castnodeexp.h"

namespace tiny {

class CASTNodeStmtRepeat : public CASTNodeStmt {
 private:
  CASTNodeStmtRepeat() noexcept : CASTNodeStmt{EASTNodeKind::kStmtRepeat}{}

  static constexpr int kRepeatPartIdx = 0;
  static constexpr int kUntilPartIdx = 1;

 public:
  virtual ~CASTNodeStmtRepeat() = default;

  virtual void setRepeatPart(CASTNodeStmt* stmt) {
    _children[kRepeatPartIdx] = stmt;
  }
  virtual CASTNodeStmt* repeatPart() const {
    return static_cast<CASTNodeStmt*>(_children[kRepeatPartIdx]);
  }
  virtual void setUntilPart(CASTNodeExp* exp) {
    _children[kUntilPartIdx] = exp;
  }
  virtual CASTNodeExp* untilPart() const {
    return static_cast<CASTNodeExp*>(_children[kUntilPartIdx]);
  }

 private:
  friend class CAST;
};

} // namespace tiny#pragma once
