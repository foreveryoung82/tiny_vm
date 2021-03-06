#pragma once
#include "tiny/castnodebase.h"

namespace tiny {

class CASTNodeExp: public CASTNodeBase {
 protected:
  CASTNodeExp(EASTNodeKind kind) : CASTNodeBase{kind} {}

 public:
  virtual ~CASTNodeExp() = default;
};

} // namespace tiny