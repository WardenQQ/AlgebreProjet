#include <assert.h>

#include "symbol_table.h"

int main()
{
  SymbolTable s = newSymbolTable();

  Data data1 = {.matrix = {.type = DATA_MATRIX, .value = newMatrix(2, 2)}};
  add_entry(s, "id_test1", data1);

  data1 = find_entry_data(s, "does_not_exist");
  assert(data1.common.type == DATA_NULL);

  data1 = find_entry_data(s, "id_test1");
  assert(data1.common.type == DATA_MATRIX);
  displayMatrix(data1.matrix.value);
}
