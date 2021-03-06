#pragma once
#include <memory>

#include "tiny/cvmopbuffer.h"

namespace tiny {
class CAST;
}

namespace tiny {

// ref class
// translater which translate abstract syntax tree into virtual machine op code
class CTranslater final {
 public:
  std::unique_ptr<CVMOpBuffer> translate(CAST& ast);

 private:
  struct SImpl;
  friend struct SImpl;
};

} // namespace tiny