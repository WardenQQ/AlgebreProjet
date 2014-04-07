#ifndef _interpreter_h_
#define _interpreter_h_

#include "tree.h"
#include "matrix.h"
#include "operation.h"

enum data_type {
  DATA_NULL,
  DATA_NUMBER,
  DATA_MATRIX
};

union data {
  struct {
    enum data_type type;
  } common;

  struct {
    enum data_type type;
    double value;
  } number;

  struct {
    enum data_type type;
    Matrix value;
  } matrix;
};

void interpreter(Tree root);

#endif //_interpreter_h_
