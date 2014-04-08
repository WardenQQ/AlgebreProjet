#include <string.h>

#include "symbol_table.h"

static void grow_symbol_table(SymbolTable s);

SymbolTable newSymbolTable()
{
  SymbolTable s = malloc(sizeof(*s));
  s->count = 0;
  s->size = 64;
  s->array = malloc(sizeof(*s->array) * s->size);
  
  return s;
}

void deleteSymbolTable(SymbolTable s)
{
  // On supprime chacune des matrices alloués
  for (int i = 0; i < s->count; ++i) {
    if (s->array[i].data.common.type == DATA_MATRIX) {
      deleteMatrix(s->array[i].data.matrix.value);
    }
  }

  free(s->array);
  free(s);
}

void add_entry(SymbolTable s, char *id, Data data)
{
  for (int i = 0; i <s->count; ++i) {
    if (strncmp(s->array[i].name, id, STRING_MAX) == 0) {
      if (s->array[i].data.common.type == DATA_MATRIX) {
        deleteMatrix(s->array[i].data.matrix.value);
      }
      s->array[i].data = data;
      return;
    }
  }

  if (s->count >= s->size) {
    grow_symbol_table(s);
  }
  strncpy(s->array[s->count].name, id, STRING_MAX);
  s->array[s->count].data = data;
}

Data find_entry_data(SymbolTable s, char *id)
{
  for (int i = 0; i < s->size; ++i) {
    if (strncmp(s->array[i].name, id, STRING_MAX) == 0) {
      return s->array[i].data;
    }
  }

  Data null = {.common = {.type = DATA_NULL}};
  return null;
}

static void grow_symbol_table(SymbolTable s)
{
  void *ptr = realloc(s->array, s->size * 2);
  if (ptr == NULL) {
    // TODO: manage error
  }
  s->size *= 2;
  s->array = ptr;
}
