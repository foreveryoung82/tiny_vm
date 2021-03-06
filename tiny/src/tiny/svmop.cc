#include "tiny/svmop.h"

using std::visit;

namespace tiny {

std::ostream& operator<<(std::ostream& os, const SVMOp& op) {
  os << '(' << op.code <<',';
  visit([&os](auto&& v) {os<<v;},
        op.argument);
  os << ')';

  return os;
}

}  // namespace tiny