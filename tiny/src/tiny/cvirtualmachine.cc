#include "tiny/cvirtualmachine.h"

#include <cassert>
#include <iostream>
#include <stack>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "gsl/gsl"
#include "tiny/cvmopbuffer.h"
//#include "tiny/cvmsymboltable.h"
#include "tiny/evmopcode.h"
#include "windy/basetype.h"

#include "windy/agility.h"

using std::cout;
using std::endl;
using std::get;
using std::make_pair;
using std::stack;
using std::string_view;
using std::unordered_map;
using std::vector;
using gsl::narrow_cast;
using windy::i32;

using stack_t=stack<i32, vector<i32>>;
using nameToVarIdx_t=unordered_map<string_view, i32>;

namespace {
using namespace tiny;

bool cmp_not_zero(i32 test) { return 0!=test; }
bool cmp_zero(i32 test) {return 0==test; }
i32 math_plus(i32 a, i32 b) {return a+b;}
i32 math_minus(i32 a, i32 b) {return a-b;}
i32 math_multiply(i32 a, i32 b) {return a*b;}
i32 math_divide(i32 a, i32 b) {return a/b;}
i32 test_equal(i32 a, i32 b) {return a==b;}
i32 test_less_than(i32 a, i32 b) {return a<b;}
} // namespace

namespace tiny {

struct CVirtualMachine::SImpl {
  static constexpr i32 kMaxOpCount = 0xffff;

  i32 _nextOpIdx;
  stack_t _stack;
  //CSymbolTable  _symbolTable;
  nameToVarIdx_t _nameToVarIdx;
  vector<i32> _variables;

  void run(const CVMOpBuffer& buf) {
    while (_nextOpIdx < buf.size()) {
      runOp(buf[_nextOpIdx]);
      ++_nextOpIdx;
    }
  }

  void runOp(const SVMOp& op) { 
    switch (op.code) {
      case EVMOpCode::kJump:
        runOpJumpUnconditional(op);
      break;

      case EVMOpCode::kJumpIfNotZero:
        runOpJumpWithCmp(op, &cmp_not_zero);
      break;

      case EVMOpCode::kJumpIfZero:
        runOpJumpWithCmp(op, &cmp_zero);
      break;

      case EVMOpCode::kLookup:
        runOpLookup(op);
      break;

      case EVMOpCode::kLookupOrNew:
        runOpLookupOrNew(op);
      break;

      case EVMOpCode::kVariableRead:
        runOpVariableRead(op);
      break;

      case EVMOpCode::kVariableWrite:
        runOpVariableWrite(op);
      break;

      case EVMOpCode::kPush:
        runOpPush(op);
      break;

      case EVMOpCode::kPop:
        runOpPop();
      break;

      case EVMOpCode::kPlus:
        runOpMath(&math_plus);
      break;

      case EVMOpCode::kMinus:
        runOpMath(&math_minus);
      break;

      case EVMOpCode::kMultiply:
        runOpMath(&math_multiply);
      break;

      case EVMOpCode::kDivide:
        runOpMath(&math_divide);
      break;

      case EVMOpCode::kEqual:
        runOpTest(&test_equal);
      break;

      case EVMOpCode::kLessThan:
        runOpTest(&test_less_than);
      break;

      case EVMOpCode::kRead:
        assert(!"Not implemented!");
      break;

      case EVMOpCode::kWrite:
        runOpWrite(op);
      break;

      case EVMOpCode::kHalt:
        runOpHalt();
      break;

    }
  }

  void runOpJumpUnconditional(const SVMOp& op) {
    // remark: every op will add one to _nextOpIdx, so minus one to avoid branch
    _nextOpIdx = get<i32>(op.argument)-1;
  }

  void runOpJumpWithCmp(const SVMOp& op, bool(*cmp)(i32)) {
    i32 test = _stack.top();
    _stack.pop();

    if (cmp(test)) {
      // remark: every op will add one to _nextOpIdx, so minus one to avoid branch
      _nextOpIdx = get<i32>(op.argument)-1;
    }
  }

  void runOpLookup(const SVMOp& op) { 
    auto name = get<string_view>(op.argument);
    i32 var_idx = _nameToVarIdx[name];
    _stack.push(var_idx);
  }

  void runOpLookupOrNew(const SVMOp& op) {
    auto name = get<string_view>(op.argument);
    auto itr = _nameToVarIdx.find(name);
    i32 var_idx = 0;

    if (_nameToVarIdx.end() == itr) {
      var_idx = _variables.size();
      _variables.push_back(0);
      _nameToVarIdx.insert(make_pair(name, var_idx));
    } else {
      var_idx = itr->second;
    }
    _stack.push(var_idx);
  }

  void runOpVariableRead(const SVMOp& op) {
    i32 var_id = _stack.top();
    _stack.pop();
    _stack.push(_variables[var_id]);
  }

  void runOpVariableWrite(const SVMOp& op) {
    i32 var_id = _stack.top();
    _stack.pop();
    i32 value = _stack.top();
    _stack.pop();
    _variables[var_id] = value;
  }

  void runOpPush(const SVMOp& op) {
    _stack.push(get<i32>(op.argument));
  }

  void runOpPop() {return _stack.pop();}

  void runOpMath(i32(*func)(i32,i32)) {
    i32 b = _stack.top();
    _stack.pop();
    i32 a = _stack.top();
    _stack.pop();
    _stack.push(func(a,b));
  }

  void runOpTest(i32(*test)(i32, i32)) { runOpMath(test); }

  void runOpWrite(const SVMOp& op) {
    auto name = get<string_view>(op.argument);
    i32 var_idx = _nameToVarIdx[name];
    i32 value = _variables[var_idx];
    cout<<value<<endl;
  }

  void runOpHalt() { _nextOpIdx = kMaxOpCount; }
};

CVirtualMachine::CVirtualMachine() noexcept : _pImpl{new SImpl{0}} {}

CVirtualMachine::~CVirtualMachine()=default;

void CVirtualMachine::run(const CVMOpBuffer& buf) {
  assert(EVMOpCode::kHalt==buf[buf.size()-1].code);

  _pImpl->run(buf);
}

}  // namespace tiny