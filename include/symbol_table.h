#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <stdbool.h>

#include "matrix.h"

#define STRING_MAX 256

enum data_type {
  DATA_NULL,
  DATA_NUMBER,
  DATA_MATRIX
};

typedef union data {
  struct {
    enum data_type type;
  } common;

  struct {
    enum data_type type;
    double value;
  } number;

  struct {
    enum data_type type;
    bool is_temp;
    Matrix value;
  } matrix;
} Data;

typedef struct entry {
  char name[STRING_MAX];
  Data data;
} Entry;

typedef struct symbol_table {
  int count;
  int size;
  Entry *array;
} *SymbolTable;


SymbolTable newSymbolTable();
void deleteSymbolTable();
void add_entry(SymbolTable s, char *id, Data data);
Data find_entry_data(SymbolTable s, char *id);

#endif //_SYMBOL_TABLE_H_
