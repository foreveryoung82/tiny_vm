#pragma once
#include <memory>

namespace tiny {

class CAST;
class CLexer;

} // namespace tiny

namespace tiny {

// ref class
// parser for tiny vm
class CParser final {
 public:
  std::unique_ptr<CAST> parse(CLexer& lexer);

 private:
  struct SImpl;
  friend struct SImpl;
};

}  // namespace tiny