#pragma once
#include <string_view>

#include "tiny/castnodeexp.h"

namespace tiny {

class CASTNodeExpNum: public CASTNodeExp {
 private:
  CASTNodeExpNum() noexcept : CASTNodeExp{EASTNodeKind::kExpNum}{}

 public:
  virtual ~CASTNodeExpNum() = default;

  void setValue(std::string_view new_value) noexcept {_value=new_value;}
  std::string_view value() const noexcept { return _value; }

 private:
  friend class CAST;
};

} // namespace tiny