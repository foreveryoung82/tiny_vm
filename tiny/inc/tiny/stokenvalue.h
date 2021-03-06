#pragma once
#include <string_view>
#include <variant>

namespace tiny {

// value class with cheap ctor, copy, move
// struct for holding variant value of a token
struct STokenValue {
  const std::string_view& lexeme() const {
    return std::get<std::string_view>(variant);
  }
  std::string_view& lexeme() { 
    return std::get<std::string_view>(variant); 
  }
  void setLexeme(std::string_view lexeme) {variant = lexeme;}

  std::variant<std::string_view> variant;
};

}  // namespace tiny