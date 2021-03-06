#pragma once
#include <array>

#include "tiny/castnodebase.h"

namespace tiny {

class CASTNodeStmt: public CASTNodeBase {
 protected:
  CASTNodeStmt(EASTNodeKind kind) noexcept : CASTNodeBase(kind) {}

 public:
  virtual ~CASTNodeStmt() = default;
};

} // namespace tiny