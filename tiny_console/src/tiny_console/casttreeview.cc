#include "tiny_console/casttreeview.h"

#include <sstream>
#include <string>

#include "tiny/cast.h"
#include "tiny/castnoderoot.h"
#include "tiny/castnodestmt.h"
#include "tiny/castnodestmtif.h"
#include "tiny/castnodestmtrepeat.h"
#include "tiny/castnodestmtassign.h"
#include "tiny/castnodestmtread.h"
#include "tiny/castnodestmtwrite.h"
#include "tiny/castnodeexpop.h"
#include "tiny/castnodeexpid.h"
#include "tiny/castnodeexpnum.h"
#include "tiny/etokenkind.h"
#include "tiny/iastnode.h"
#include "tiny_console/tinyxml2.h"

using std::string;
using std::string_view;
using std::stringstream;
using tiny::EASTNodeKind;
using tiny::CASTNodeStmtAssign;
using tiny::CASTNodeStmtIf;
using tiny::CASTNodeStmtRepeat;
using tiny::CASTNodeStmtRead;
using tiny::CASTNodeStmtWrite;
using tiny::CASTNodeExpOp;
using tiny::CASTNodeExpID;
using tiny::CASTNodeExpNum;
using tiny::IASTNode;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLPrinter;
using tinyxml2::XMLNode;

namespace {

struct SVisitContext {
  XMLDocument& doc;
};

string format_ast_node_kind(EASTNodeKind kind) noexcept {
  stringstream ss {};
  ss << kind;

  return ss.str();
}

void visit(const IASTNode& ast_node,
           XMLNode& xml_parent,
           SVisitContext& context) {
  XMLDocument& doc = context.doc;
  string node_name = format_ast_node_kind(ast_node.kind());
  XMLElement* xml_node = doc.NewElement(node_name.c_str());

  string_view value = ast_node.value();
  if (!value.empty()) {
    stringstream ss {};
    ss << ast_node.value();
    xml_node->SetAttribute("value", ss.str().c_str());
  }
  xml_parent.InsertEndChild(xml_node);

  for (IASTNode* child : ast_node.children()) {
    if (!child) break;

    visit(*child, *xml_node, context);

    for (IASTNode* sibling = child->sibling();
         sibling;
         sibling = sibling->sibling()) {
      visit(*sibling, *xml_node, context);
    }
  }
}

} // namespace

namespace tiny_console {

CASTTreeView::CASTTreeView(const tiny::CAST& ast) noexcept
    : _xmlPrinter{new XMLPrinter{}} 
{
  XMLDocument doc{};
  doc.InsertFirstChild(doc.NewElement("xml"));
  SVisitContext context{doc};
  visit(ast.root(), *doc.RootElement(), context);
  doc.Print(_xmlPrinter.get());
}

CASTTreeView::~CASTTreeView() noexcept = default;

std::string_view CASTTreeView::asXml() const {
  return string_view(_xmlPrinter->CStr());
}

}  // namespace tiny_console