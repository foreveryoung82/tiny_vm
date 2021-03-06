#pragma once
#include <memory>
#include <vector>

namespace tiny {

class IASTNode;
class CASTNodeRoot;
class CASTNodeStmt;
class CASTNodeStmtIf;
class CASTNodeStmtRepeat;
class CASTNodeStmtAssign;
class CASTNodeStmtRead;
class CASTNodeStmtWrite;
class CASTNodeExpOp;
class CASTNodeExpNum;
class CASTNodeExpID;

} // namespace tiny

namespace tiny {

// ref class
// abstract syntax tree for tiny vm
class CAST final {
 public:
  CAST() noexcept;
  ~CAST() noexcept;

  CASTNodeRoot& root();
  const CASTNodeRoot& root() const;
  void setEntry(IASTNode * entry);
  IASTNode* entry() const;

  // create ast node
  CASTNodeStmtIf* createStmtIf();
  CASTNodeStmtRepeat* createStmtRepeat();
  CASTNodeStmtAssign* createStmtAssign();
  CASTNodeStmtRead* createStmtRead();
  CASTNodeStmtWrite* createStmtWrite();
  CASTNodeExpOp* createExpOp();
  CASTNodeExpNum* createExpNum();
  CASTNodeExpID* createExpID();

 private:
  std::vector<std::unique_ptr<IASTNode>> _nodes;
};

} // namespace tiny