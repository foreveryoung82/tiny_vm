#include "tiny/cast.h"

#include "tiny/castnodeexpid.h"
#include "tiny/castnodeexpnum.h"
#include "tiny/castnodeexpop.h"
#include "tiny/castnoderoot.h"
#include "tiny/castnodestmtassign.h"
#include "tiny/castnodestmtif.h"
#include "tiny/castnodestmtread.h"
#include "tiny/castnodestmtrepeat.h"
#include "tiny/castnodestmtwrite.h"

using std::unique_ptr;

namespace tiny {

CAST::CAST() noexcept : _nodes{} { _nodes.emplace_back(new CASTNodeRoot{}); }

CAST::~CAST() {}

CASTNodeRoot& CAST::root() { return static_cast<CASTNodeRoot&>(*_nodes[0]); }

const CASTNodeRoot& CAST::root() const { 
  return static_cast<const CASTNodeRoot&>(*_nodes[0]); 
}

void CAST::setEntry(IASTNode* entry) { root().setChild(entry); }

IASTNode* CAST::entry() const { return root().child(); }

// create ast node
CASTNodeStmtIf* CAST::createStmtIf() {
  auto& node = _nodes.emplace_back(new CASTNodeStmtIf{});

  return static_cast<CASTNodeStmtIf*>(node.get());
}

CASTNodeStmtRepeat* CAST::createStmtRepeat() {
  auto& node = _nodes.emplace_back(new CASTNodeStmtRepeat{});

  return static_cast<CASTNodeStmtRepeat*>(node.get());
}

CASTNodeStmtAssign* CAST::createStmtAssign() {
  auto& node = _nodes.emplace_back(new CASTNodeStmtAssign{});

  return static_cast<CASTNodeStmtAssign*>(node.get());
}

CASTNodeStmtRead* CAST::createStmtRead() {
  auto& node = _nodes.emplace_back(new CASTNodeStmtRead{});

  return static_cast<CASTNodeStmtRead*>(node.get());
}

CASTNodeStmtWrite* CAST::createStmtWrite() {
  auto& node = _nodes.emplace_back(new CASTNodeStmtWrite{});

  return static_cast<CASTNodeStmtWrite*>(node.get());
}

CASTNodeExpOp* CAST::createExpOp() {
  auto& node = _nodes.emplace_back(new CASTNodeExpOp{});

  return static_cast<CASTNodeExpOp*>(node.get());
}

CASTNodeExpNum* CAST::createExpNum() {
  auto& node = _nodes.emplace_back(new CASTNodeExpNum{});

  return static_cast<CASTNodeExpNum*>(node.get());
}

CASTNodeExpID* CAST::createExpID() {
  auto& node = _nodes.emplace_back(new CASTNodeExpID{});

  return static_cast<CASTNodeExpID*>(node.get());
}

}  // namespace tiny