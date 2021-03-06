#pragma once
#include <string_view>

namespace tiny_console {

// ref class
// an interactive console for tiny vm
class CConsole {
 public:
  void run();

 private:
  void _process(std::string_view src);
};

}  // namespace tiny_console