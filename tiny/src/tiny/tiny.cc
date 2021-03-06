#pragma once
#include "tiny/tiny.h"
#include "tiny/eastnodekind.h"
#include "tiny/etokenkind.h"
#include "tiny/evmopcode.h"

namespace tiny {

void initialize() {
  initialze_eastnodekind();
  initialze_etokenkind();
  initialze_evmopcode();
}

void finalize() { 
  finalize_evmopcode();
  finalize_etokenkind();
  finalize_eastnodekind();
}

}  // namespace tiny