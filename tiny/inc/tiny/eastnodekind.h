#pragma once
#include <ostream>

#include "windy/basetype.h"

namespace tiny {

// enum for distinguishing abstract syntax tree node kind
// u8 for 'SWITCH' control structure optimization of C/C++ compiler
// first 3 bits tell whether it is stmt/exp/other,
// remained 5 bits tell its sub kind
enum class EASTNodeKind : windy::u8 {
  _kRoot = 0x0,
  _kStmt,
  _kExp,

  _kShiftBit = 5,
  _kFirstKindMask = 0x7<<_kShiftBit,
  _kStmtXorTest = _kStmt<<_kShiftBit,
  _kExpXorTest = _kExp<<_kShiftBit,

  // root
  kRoot = _kRoot,

  // stmt
  kStmtIf = _kStmt<<_kShiftBit,
  kStmtRepeat,
  kStmtAssign,
  kStmtRead,
  kStmtWrite,

  // exp
  kExpOp = _kExp<<_kShiftBit,
  kExpID,
  kExpNum
};

inline bool is_stmt(EASTNodeKind node_kind) {
  using windy::u8;

  constexpr auto test = static_cast<u8>(EASTNodeKind::_kStmtXorTest);
  const auto kind = static_cast<u8>(node_kind);

  return !(test^kind); 
}

inline bool is_exp(EASTNodeKind node_kind) {
  using windy::u8;

  constexpr auto test = static_cast<u8>(EASTNodeKind::_kExpXorTest);
  const auto kind = static_cast<u8>(node_kind);

  return !(test^kind);
}

std::ostream& operator<<(std::ostream& os, EASTNodeKind kind);

void initialze_eastnodekind();
void finalize_eastnodekind();

} // namespace tiny