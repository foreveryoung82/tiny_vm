#pragma once
#include "tiny/castnodestmt.h"

namespace tiny {

class CASTNodeStmtRead: public CASTNodeStmt {
 private:
  CASTNodeStmtRead() noexcept : CASTNodeStmt{EASTNodeKind::kStmtRead}{}

 public:
  virtual ~CASTNodeStmtRead()=default;

  void setIdentifier(std::string_view id) noexcept {_value = id;}

 private:
  friend class CAST;
};

} // namespace tiny