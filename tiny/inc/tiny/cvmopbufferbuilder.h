#pragma once
#include <string_view>
#include <vector>

#include "windy/basetype.h"
#include "tiny/cvmlabel.h"
#include "tiny/cvmopbuffer.h"
#include "tiny/evmopcode.h"

namespace tiny {

// ref class
// builder for virtual machine op buffer
class CVMOpBufferBuilder final {
 public:
  CVMOpBufferBuilder()=default;
  ~CVMOpBufferBuilder()=default;

  CVMLabel newLabel() noexcept;
  void addLabel(const CVMLabel& label) noexcept;

  void addOpJump(CVMLabel label) noexcept;
  //void addOpJumpIfNotZero(CVMLabel label) noexcept;
  void addOpJumpIfZero(CVMLabel label) noexcept;

  void addOpLookup(std::string_view name) noexcept {
    _pBuffer->add(SVMOp{name, EVMOpCode::kLookup});
  }
  void addOpLookupOrNew(std::string_view name) noexcept {
    _pBuffer->add(SVMOp{name, EVMOpCode::kLookupOrNew});
  }

  void addOpVariableRead() noexcept {
    _pBuffer->add(SVMOp{{}, EVMOpCode::kVariableRead});
  }
  void addOpVariableWrite() noexcept {
    _pBuffer->add(SVMOp{{}, EVMOpCode::kVariableWrite});
  }

  void addOpPush(windy::i32 num) noexcept {
    _pBuffer->add(SVMOp{num, EVMOpCode::kPush});
  }
  void addOpPop() noexcept {
    _pBuffer->add(SVMOp{{}, EVMOpCode::kPop});
  }

  void addOpPlus() noexcept {
    _pBuffer->add(SVMOp{{}, EVMOpCode::kPlus});
  }
  void addOpMinus() noexcept {
    _pBuffer->add(SVMOp{{}, EVMOpCode::kMinus});
  }
  void addOpMultiply() noexcept {
    _pBuffer->add(SVMOp{{}, EVMOpCode::kMultiply});
  }
  void addOpDivide() noexcept {
    _pBuffer->add(SVMOp{{}, EVMOpCode::kDivide});
  }

  void addOpEqual() noexcept {
    _pBuffer->add(SVMOp{{}, EVMOpCode::kEqual});
  }
  void addOpLessThan() noexcept {
    _pBuffer->add(SVMOp{{}, EVMOpCode::kLessThan});
  }

  void addOpRead(std::string_view name) noexcept {
    _pBuffer->add(SVMOp{name, EVMOpCode::kRead});
  }
  void addOpWrite(std::string_view name) noexcept {
    _pBuffer->add(SVMOp{name, EVMOpCode::kWrite});
  }

  void start() noexcept;
  // complete() adds tailing halt op (which is required by virtual machine)
  //  to op buffer
  std::unique_ptr<CVMOpBuffer> complete() noexcept;

 private:
  std::unique_ptr<CVMOpBuffer> _pBuffer;
  std::vector<windy::i32> _jumpEntries;
  std::vector<windy::i32> _jumpOpIndices;
};

} // namespace tiny