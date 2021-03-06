#include "tiny/csymboltable.h"

#include <functional>

#include "tiny/ssymboltableentry.h"

using std::make_pair;
using std::unique_ptr;

namespace tiny {

CSymbolTable::CSymbolTable() noexcept 
  : _pNameToEntry{new nameToEntry_t{}} {}

SSymbolTableEntry* CSymbolTable::lookup(std::string_view name) {
  auto itr = _pNameToEntry->find(name);
  if (_pNameToEntry->end()==itr)
    return nullptr;
  else
    return &itr->second;
}

bool CSymbolTable::insert(std::string_view name,
                          const SSymbolTableEntry& entry) {
  auto insert_result = _pNameToEntry->insert(make_pair(name, entry));
  return insert_result.second;
}

} // namespace tiny