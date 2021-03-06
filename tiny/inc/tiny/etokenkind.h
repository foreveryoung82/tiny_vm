#pragma once
#include <ostream>

#include "windy/basetype.h"

namespace tiny {

enum class ETokenKind : windy::i32 {
  // keywords
  kIf = 1,
  kThen,
  kElse,
  kEnd,
  kRepeat,
  kUntil,
  kRead,
  kWrite,

  // operators
  kPlus = 101,
  kMinus,
  kMultiply,
  kDivide,
  kEqual, // =
  kLessThan,
  kLeftBrace,
  kRightBrace,
  kSemicolon,
  kAssign, // :=

  // literal and identifier
  kNumber = 201, // 1 or more digits
  kIdentifier, //  1 or more letters
  
  // special
  kEOS = 301, // end of whole string 
  kError, // error when scaning token
};

// output token in literal form
std::ostream& operator<<(std::ostream& os, ETokenKind token);

void initialze_etokenkind();
void finalize_etokenkind();
} // namespace tiny