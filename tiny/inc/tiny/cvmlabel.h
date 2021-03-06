#pragma once
#include "windy/basetype.h"

namespace tiny {

// value class, cheap copy/move/ct/dt
// jump entry idx for control structure of tiny vm op buffer
class CVMLabel final {
 public:
  explicit CVMLabel(windy::i32 idx) noexcept: _jumpEntryIdx{idx} {}
  ~CVMLabel()=default;
  CVMLabel(const CVMLabel& rhs)=default;
  CVMLabel(CVMLabel&& rhs)=default;
  CVMLabel& operator=(const CVMLabel& rhs)=default;
  CVMLabel& operator=(CVMLabel&& rhs)=default;

  windy::i32 jumpEntryIdx() const noexcept {return _jumpEntryIdx;}

 private:
  windy::i32 _jumpEntryIdx;
};

} // namespace tiny