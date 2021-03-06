#pragma once
#include <ostream>
#include <string_view>
#include <variant>

#include "tiny/evmopcode.h"

namespace tiny {

// value class, cheap copy/move/ct/dt
// op command for virtual machine
struct SVMOp {
  std::variant<int, std::string_view> argument;
  EVMOpCode code;
};

std::ostream& operator<<(std::ostream& os, const SVMOp& op);

} // namespace tiny