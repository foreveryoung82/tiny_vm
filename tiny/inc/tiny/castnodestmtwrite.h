#pragma once
#include "tiny/castnodestmt.h"

namespace tiny {

class CASTNodeStmtWrite: public CASTNodeStmt {
 private:
  CASTNodeStmtWrite() noexcept : CASTNodeStmt{EASTNodeKind::kStmtWrite}{}

 public:
  virtual ~CASTNodeStmtWrite()=default;

  void setIdentifier(std::string_view id) noexcept {_value = id;}

 private:
  friend class CAST;
};

} // namespace tiny#pragma once
