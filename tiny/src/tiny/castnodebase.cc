#include "tiny/castnodebase.h"

namespace tiny {
CASTNodeBase::CASTNodeBase(EASTNodeKind kind) noexcept
  : _kind{kind},
    _sibling{nullptr},
    _children{nullptr}, 
    _value{} {
}

EASTNodeKind CASTNodeBase::kind() const { return _kind; }

IASTNode* CASTNodeBase::sibling() const { return _sibling; }

gsl::span<IASTNode* const> CASTNodeBase::children() const {
  return gsl::span<IASTNode* const>(_children);
}

std::string_view CASTNodeBase::value() const { return _value; }

} // namespace tiny