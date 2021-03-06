#include "cvmopbufferbuilder.h"

#include "gsl/gsl"

using std::get;
using std::unique_ptr;
using gsl::narrow_cast;
using windy::i32;

namespace tiny {

CVMLabel CVMOpBufferBuilder::newLabel() noexcept {
  i32 entry_idx = narrow_cast<i32>(_jumpEntries.size());
  _jumpEntries.emplace_back(0);

  return CVMLabel{entry_idx};
}

void CVMOpBufferBuilder::addLabel(const CVMLabel& label) noexcept {
  _jumpEntries[label.jumpEntryIdx()] = narrow_cast<i32>(_pBuffer->size());
}

void CVMOpBufferBuilder::addOpJump(CVMLabel label) noexcept {
  i32 op_idx = narrow_cast<i32>(_pBuffer->size());
  _jumpOpIndices.push_back(op_idx);

  _pBuffer->add(SVMOp{label.jumpEntryIdx(), EVMOpCode::kJump});
}

void CVMOpBufferBuilder::addOpJumpIfZero(CVMLabel label) noexcept {
  i32 op_idx = narrow_cast<i32>(_pBuffer->size());
  _jumpOpIndices.push_back(op_idx);

  _pBuffer->add(SVMOp{label.jumpEntryIdx(), EVMOpCode::kJumpIfZero});
}

void CVMOpBufferBuilder::start() noexcept {
  _pBuffer.reset(new CVMOpBuffer{});
  _jumpEntries.resize(0);
  _jumpOpIndices.resize(0);
}

std::unique_ptr<CVMOpBuffer> CVMOpBufferBuilder::complete() noexcept { 
  _pBuffer->add(SVMOp{{}, EVMOpCode::kHalt}); // tailing halt

  for (i32 op_idx:_jumpOpIndices) {
    SVMOp& op = (*_pBuffer)[op_idx];
    op.argument = _jumpEntries[get<i32>(op.argument)];
  }

  return std::move(_pBuffer);
}

} // namespace tiny
