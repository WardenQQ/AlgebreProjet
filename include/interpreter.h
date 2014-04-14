#ifndef _interpreter_h_
#define _interpreter_h_

#include "tree.h"
#include "matrix.h"
#include "operation.h"
#include "symbol_table.h"

bool interpreter(Tree root, SymbolTable symbol_table);

#endif //_interpreter_h_
