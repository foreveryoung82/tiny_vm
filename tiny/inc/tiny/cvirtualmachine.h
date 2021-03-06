#pragma once
#include <memory>

namespace tiny {
class CVMOpBuffer;
} // namespace tiny

namespace tiny {

// ref class
// virtual stack machine for tiny language
class CVirtualMachine final {
 public:
  CVirtualMachine() noexcept;
  ~CVirtualMachine() noexcept;

  void run(const CVMOpBuffer& buf);

 private:
  struct SImpl;
  std::unique_ptr<SImpl> _pImpl;
};

} // namespace tiny