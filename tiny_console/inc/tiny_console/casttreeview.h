#pragma once
#include <string_view>
#include <memory>

namespace tiny {
class CAST;
} // namespace tiny

namespace tinyxml2 {
class XMLPrinter;
} // namespace tinyxml2

namespace tiny_console {

class CASTTreeView {
 public:
  CASTTreeView(const tiny::CAST& ast) noexcept;
  virtual ~CASTTreeView() noexcept;

  std::string_view asXml() const;

 private:
  std::unique_ptr<tinyxml2::XMLPrinter> _xmlPrinter;
};

} // namespace tiny_console
