#pragma once
#include "tiny/castnodebase.h"

namespace tiny {

// ref class
// root node for tiny vm
// root node has only one child and it must be entry of whole program
class CASTNodeRoot: public CASTNodeBase {
 private:
  CASTNodeRoot() noexcept : CASTNodeBase{EASTNodeKind::_kRoot}{}
  
 public:
  virtual ~CASTNodeRoot()=default;

  void setChild(IASTNode* child) noexcept { _children[0]=child; }
  IASTNode* child() const noexcept { return _children[0]; }

 private:
  friend class CAST;
};

} // namespace tiny