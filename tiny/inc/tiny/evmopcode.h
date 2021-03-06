#pragma once
#include <ostream>

#include "windy/basetype.h"

#include "windy/agility.h"

namespace tiny {

enum class EVMOpCode: windy::u8 {
  // no operation (only for debug)
  // (NOP, None)
  kNOP,
  // halt virtual machine
  // (HALT, None)
  kHalt,

  // control flow
  // (JUMP, label): unconditional jump
  //   PC = label
  kJump,
  // (JUMP_IF_NOT_ZERO, label): jump if not 0
  //   if pop():
  //       PC = label.op_address
  kJumpIfNotZero,
  // (JUMP_IF_ZERO, label): jump if 0
  //   if not pop():
  //       PC = label.op_address
  kJumpIfZero,

  // name lookup
  // (LOOKUP, name): loopup name string and push id
  //   id = NAME.get(name)
  //   push(id)
  kLookup,
  // (LOOKUP_OR_NEW, name): if name exsits, works like LOOKUP;
  //                        if not, create new variable with name first.
  //   id = NAME.get(name)
  //   if not id:
  //       id = new_variable(name)
  //       NAME[name] = id
  //   push(id)
  kLookupOrNew,

  // variable access
  // (VARIABLE_READ, None): read from variable which id is S[-1]
  //   id = pop()
  //   push(VARIABLE[id])
  kVariableRead,
  // (VARIABLE_WRITE, None): write to variable which id is S[-1], value is S[-2]
  //   id = pop()
  //   VARIABLE[id] = pop()
  kVariableWrite,

  // stack manipulation
  // (PUSH, object): push object to stack top
  //   push(object)
  kPush,
  // (POP, None): pop stack
  //   pop()
  kPop,

  // arithmetic operation
  // (PLUS, None): s[-2:] = s[-2]+s[-1]
  kPlus,
  // (MINUS, None): s[-2:] = s[-2]-s[-1]
  kMinus,
  // (MULTIPLY, None): s[-2:] = s[-2]*s[-1]
  kMultiply,
  // (DIVIDE, None): s[-2:] = s[-2]/s[-1]
  kDivide,

  // comparision operation
  // (EQUAL, None): compare if S[-1]==S[-2] and push result
  // S[-2:] = S[-2]==S[-1]
  kEqual,
  // (LESS_THAN)
  // (LESS_THAN, None): compare if S[-2]<S[-1] and push result
  // S[-2:] = S[-2]<S[-1]
  kLessThan,

  // I/O operations
  // (READ, name): read number into variable by name
  kRead,
  // (WRITE, name): write variable by name to screen
  kWrite,
};

std::ostream& operator<<(std::ostream& os, EVMOpCode code);

void initialze_evmopcode();
void finalize_evmopcode();
} // namespace tiny