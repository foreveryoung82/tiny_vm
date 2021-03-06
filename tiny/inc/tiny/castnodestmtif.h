#pragma once
#include "tiny/castnodestmt.h"
#include "tiny/castnodeexp.h"

namespace tiny {
class CASTNodeExp;
} // namespace tiny

namespace tiny {

class CASTNodeStmtIf: public CASTNodeStmt {
 private:
  CASTNodeStmtIf() noexcept : CASTNodeStmt{EASTNodeKind::kStmtIf}{}

  static constexpr int kIfPartIdx = 0;
  static constexpr int kThenPartIdx = 1;
  static constexpr int kElsePartIdx = 2;

 public:
  virtual ~CASTNodeStmtIf()=default;

  void setIfPart(CASTNodeExp* exp) { _children[kIfPartIdx] = exp; }
  CASTNodeExp* ifPart() const {
    return static_cast<CASTNodeExp*>(_children[kIfPartIdx]);
  }

  void setThenPart(CASTNodeStmt* stmt) { _children[kThenPartIdx] = stmt; }
  CASTNodeStmt* thenPart() const {
    return static_cast<CASTNodeStmt*>(_children[kThenPartIdx]);
  }

  void setElsePart(CASTNodeStmt* stmt) { _children[kElsePartIdx] = stmt; }
  CASTNodeStmt* elsePart() const {
    return static_cast<CASTNodeStmt*>(_children[kElsePartIdx]);
  }

 private:
  friend class CAST;
};

} // namespace tiny