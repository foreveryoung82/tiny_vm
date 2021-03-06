#pragma once
#include <string_view>

#include "tiny/castnodeexp.h"

namespace tiny {

class CASTNodeExpID: public CASTNodeExp {
 private:
  CASTNodeExpID() noexcept : CASTNodeExp{EASTNodeKind::kExpID} {}

 public:
  virtual ~CASTNodeExpID() = default;

  void setName(std::string_view name) noexcept {_value = name;}
  std::string_view name() const noexcept { return _value; }

 private:
  friend class CAST;
};

} // namespace tiny