#pragma once
#include "tiny/castnodeexp.h"
#include "tiny/etokenkind.h"

namespace tiny {

class CASTNodeExpOp: public CASTNodeExp {
 private:
  CASTNodeExpOp() noexcept : CASTNodeExp{EASTNodeKind::kExpOp}, _token{} {}

  static constexpr int kLeftPartIdx = 0;
  static constexpr int kRightPartIdx = 1;

 public:
  virtual ~CASTNodeExpOp() = default;

  void setOpToken(ETokenKind token) {_token = token;}
  ETokenKind opToken() const {return _token;}

  // virtual ETokenKind token() const;
  void setLeftPart(CASTNodeExp* left) { _children[kLeftPartIdx] = left; }
  CASTNodeExp* leftPart() const {
    return static_cast<CASTNodeExp*>(_children[kLeftPartIdx]);
  }
  void setRightPart(CASTNodeExp* right) { _children[kRightPartIdx] = right; }
  CASTNodeExp* rightPart() const {
    return static_cast<CASTNodeExp*>(_children[kRightPartIdx]);
  }

 private:
  ETokenKind _token;

  friend class CAST;
};

} // namespace tiny