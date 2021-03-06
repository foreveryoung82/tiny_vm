#pragma once
#include <memory>
#include <string_view>
#include <unordered_map>

namespace tiny {

struct SSymbolTableEntry;

} // namespace

namespace tiny {

// ref class
// symbol table for tiny vm
class CSymbolTable final {
 public:
  CSymbolTable() noexcept;
  ~CSymbolTable() = default;

  // return nullptr if not found
  SSymbolTableEntry* lookup(std::string_view name);
  // return true if succeeded
  bool insert(std::string_view name, const SSymbolTableEntry& entry);

 private:
  using nameToEntry_t = std::unordered_map<std::string_view, SSymbolTableEntry>;
  std::unique_ptr<nameToEntry_t> _pNameToEntry;
};

} // namespace tiny