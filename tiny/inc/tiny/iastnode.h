#pragma once

#include <string_view>
#include <gsl/span>

#include "tiny/eastnodekind.h"

namespace tiny {

// interface class
// abstract syntax tree node for tiny vm
class IASTNode {
 public:
  static constexpr int kMaxChildrenCount = 3;
  
 public:
  virtual ~IASTNode() = default;

  virtual EASTNodeKind kind() const = 0;
  virtual IASTNode* sibling() const = 0;
  virtual gsl::span<IASTNode* const> children() const = 0;
  virtual std::string_view value() const = 0;
};

} // namespace tiny