#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>


#include "parser.h"
#include "symbol_table.h"
#include "tokens.h"
#include "tree.h"
#include "interpreter.h"

int main(int argc, char *argv[])
{
  if (argc >= 2) {
    close(0);
    open(argv[1], O_RDONLY);
  }

  SymbolTable symbol_table = newSymbolTable();

  Token lookahead;
  do {
    printf("> ");
    fflush(stdout);
    lookahead = gettoken();
    if (!isatty(0)) {
      printf("\n");
    }

    Tree root = parser(&lookahead);
    if (root != NULL) {
      interpreter(root, symbol_table);
      deleteTree(root);
    }
  } while (lookahead.type != TOK_EOF);

  deleteSymbolTable(symbol_table);

  return 0;
}
