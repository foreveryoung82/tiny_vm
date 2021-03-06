#pragma once
#include <array>

#include "tiny/eastnodekind.h"
#include "tiny/iastnode.h"

namespace tiny {

class CASTNodeBase: public IASTNode {
 protected:
  CASTNodeBase(EASTNodeKind kind) noexcept;

 public:
  virtual ~CASTNodeBase() = default;

  void setSibling(IASTNode* sibling) { _sibling = sibling; }

  // inherit from IASTNode
  virtual EASTNodeKind kind() const override;
  virtual IASTNode* sibling() const override;
  virtual gsl::span<IASTNode* const> children() const override;
  virtual std::string_view value() const override;

 protected:
  EASTNodeKind _kind;
  IASTNode* _sibling;
  std::array<IASTNode*, kMaxChildrenCount> _children;
  std::string_view _value;
};

} // namespace tiny