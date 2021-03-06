#include "tiny_console/cconsole.h"

#include <iostream>
#include <memory>
#include <string>

#include "tiny/cast.h"
#include "tiny/clexer.h"
#include "tiny/cparser.h"
#include "tiny/ctranslater.h"
#include "tiny/cvirtualmachine.h"
#include "tiny/cvmopbuffer.h"
#include "tiny/svmop.h"
#include "tiny_console/casttreeview.h"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::string_view;
using std::unique_ptr;
using tiny::CAST;
using tiny::CLexer;
using tiny::CParser;
using tiny::CTranslater;
using tiny::CVirtualMachine;
using tiny::CVMOpBuffer;
using tiny::SVMOp;

namespace {

using namespace tiny_console;

void print_ast_as_xml(const CAST& ast) {
  CASTTreeView xml_view{ast};
  cout<<xml_view.asXml()<<endl;
}

void print_op_buffer(const CVMOpBuffer& buf) {
  int op_idx = 0;
  for (const SVMOp& op:buf) {
    cout<<op_idx<<" : "<<op<<endl;
    ++op_idx;
  }
}

} // namespace

namespace tiny_console {

void CConsole::run() {
  string line{};
  while (true) {
    cout << "Enter tiny program:" << endl;
    getline(cin, line);
    if (0 == line.compare("exit")) break;

    constexpr char tailing_zero = '\0'; // lexer requirement
    line.push_back(tailing_zero);

    _process(line);
  }
}

void CConsole::_process(std::string_view src) {
  CLexer lexer{src};
  CParser parser{};
  unique_ptr<CAST> ast = parser.parse(lexer);

  cout<<"\n";
  cout<<"AST in XML format:"<<endl;
  print_ast_as_xml(*ast);

  CTranslater translater {};
  unique_ptr<CVMOpBuffer> buf = translater.translate(*ast);
  
  cout<<"\n";
  cout<<"Generated op buffer:"<<endl;
  print_op_buffer(*buf);

  cout<<"\n";
  cout<<"Program output:"<<endl;
  CVirtualMachine vm {};
  vm.run(*buf);
 }

}  // namespace tiny_console