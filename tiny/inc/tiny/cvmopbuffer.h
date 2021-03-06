#pragma once
#include <vector>

#include "gsl/gsl"
#include "windy/basetype.h"
#include "tiny/svmop.h"

namespace tiny {

// value class, handle, sequence, expensive copy, cheap move
// buffer for virtual machine op code
class CVMOpBuffer final {
 public:
  // add op into buffer and return its idx
  windy::i32 add(const SVMOp& op) noexcept {
    auto idx = gsl::narrow_cast<windy::i32>(_ops.size());
    _ops.push_back(op);
    return idx;
  }
  
  windy::i32 size() const noexcept {
    return gsl::narrow_cast<windy::i32>(_ops.size());
  }

  const SVMOp& operator[](windy::i32 idx) const noexcept { return _ops[idx]; }
  SVMOp& operator[](windy::i32 idx) noexcept { return _ops[idx]; }

  const SVMOp* begin() const noexcept { return &_ops[0]; }
  SVMOp* begin() noexcept { return &_ops[0]; }

  const SVMOp* end() const noexcept { return begin()+size(); }
  SVMOp* end() noexcept { return begin()+size(); }

 private:
  std::vector<SVMOp> _ops;
};

} // namespace tiny