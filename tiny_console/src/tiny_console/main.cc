#define _CRTDBG_MAP_ALLOC  // check memory leak
#include <stdlib.h>  
#include <crtdbg.h>  

#include <cassert>
#include <iostream>

#include "tiny/tiny.h"
#include "tiny_console/cconsole.h"

using tiny_console::CConsole;

int main() {
  tiny::initialize();
  CConsole con{};
  con.run();
  tiny::finalize();

  assert(!_CrtDumpMemoryLeaks()); // check memory leak

  return 0;
}