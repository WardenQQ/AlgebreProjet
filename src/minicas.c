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

  bool display_prompt = isatty(0);
  SymbolTable symbol_table = newSymbolTable();

  Token lookahead;
  do {
    if (display_prompt) {
      printf("> ");
      fflush(stdout);
    } else {
      printf("\n");
    }

    lookahead = gettoken();
    Tree root = parser(&lookahead);
    interpreter(root, symbol_table);
  } while (lookahead.type != TOK_EOF);

  return 0;
}
