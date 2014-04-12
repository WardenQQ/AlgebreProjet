#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "parser.h"

int main(int argc, char *argv[])
{
  if (argc >= 2) {
    close(0);
    open(argv[1], O_RDONLY);
  }

  //SymbolTable symbol_table = newSymbolTable();
  parsing_loop();
}
