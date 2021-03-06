#pragma once
#include "tiny/etokenkind.h"
//#include "tiny/stokenvalue.h"

namespace tiny {

// value class, cheap to copy/move
// entry for symbol table of tiny vm
struct SSymbolTableEntry {
  ETokenKind token;
  //STokenValue value;
};

} // namespace tiny