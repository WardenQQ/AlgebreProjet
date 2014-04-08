#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

static union data extract_data(Tree node);
static union data call_function(Tree node);
static union data call_matrix(Tree node);
static union data call_addition(Tree node);
static union data call_sub(Tree node);
static union data call_mult(Tree node);
static union data call_mult_scal(Tree node);
static union data call_expo(Tree node);
static union data call_transpose(Tree node);
static union data call_determinant(Tree node);
static union data call_invert(Tree node);
static union data call_solve(Tree node);
static union data call_rank(Tree node);

void interpreter(Tree root)
{
  union data data = extract_data(root);

  switch (data.common.type) {
    case DATA_NUMBER:
      printf("%f\n", data.number.value);
      break;
    case DATA_MATRIX:
      displayMatrix(data.matrix.value);
      break;
    default:
      break;
  }

  if (data.common.type == DATA_MATRIX) {
    deleteMatrix(data.matrix.value);
  }
}

static union data extract_data(Tree node)
{
  union data data = {.common = {.type = DATA_NULL}};

  switch (node->value.type) {
    case TOK_COLON:
      break;
    case TOK_FUNCTION:
      data = call_function(node);
      break;
    case TOK_ID:
      break;
    case TOK_NUMBER:
      data.number.type = DATA_NUMBER;
      data.number.value = node->value.number.value;
      break;
    default:
      break;
  }

  return data;
}

static union data call_function(Tree node)
{
  union data data;
  if (strncmp(node->value.id.name, "matrix", STRING_MAX) == 0) {
    data = call_matrix(node);
  } else if (strncmp(node->value.id.name, "addition", STRING_MAX) == 0) {
    data = call_addition(node);
  } else if (strncmp(node->value.id.name, "sub", STRING_MAX) == 0) {
    data = call_sub(node);
  } else if (strncmp(node->value.id.name, "mult", STRING_MAX) == 0) {
    data = call_mult(node);
  } else if (strncmp(node->value.id.name, "mult_scal", STRING_MAX) == 0) {
    data = call_mult_scal(node);
  } else if (strncmp(node->value.id.name, "expo", STRING_MAX) == 0) {
    data = call_expo(node);
  } else if (strncmp(node->value.id.name, "transpose", STRING_MAX) == 0) {
    data = call_transpose(node);
  } else if (strncmp(node->value.id.name, "determinant", STRING_MAX) == 0) {
    data = call_determinant(node);
  } else if (strncmp(node->value.id.name, "invert", STRING_MAX) == 0) {
    data = call_invert(node);
  } else if (strncmp(node->value.id.name, "solve", STRING_MAX) == 0) {
    data = call_solve(node);
  } else if (strncmp(node->value.id.name, "rank", STRING_MAX) == 0) {
    data = call_rank(node);
  } else if (strncmp(node->value.id.name, "quit", STRING_MAX) == 0) {
    exit(0);
  } else {
    fprintf(stderr, "%s is not a function\n", node->value.id.name);
  }

  return data;
}

static union data call_matrix(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count < 1) {
    fprintf(stderr, "Function %s expects at least 1 argument.\n", node->value.id.name);
    return result;
  }

  int nb_rows = node->count;
  int nb_columns = node->child[0]->count;

  Matrix m = newMatrix(nb_rows, nb_columns);

  for (int i = 0; i < nb_rows; ++i) {
    Tree list = node->child[i];
    if (list->value.type != TOK_VECTOR) {
      fprintf(stderr, "Expected an argument of type list in function %s\n",
          node->value.id.name);
      return result;
    }

    if (list->count != (int)nb_columns) {
      fprintf(stderr, "List %d is of same size as list 0 in function %s\n",
          i, node->value.id.name);
      return result;
    }

    for (int j = 0; j < nb_columns; ++j) {
      union data element = extract_data(list->child[j]);
      if (element.common.type != DATA_NUMBER) {
        fprintf(stderr, "List contains a value which is not a number\n");
        return result;
      }
      setElt(m, i, j, element.number.value);
    }
  }

  result.matrix.type = DATA_MATRIX;
  result.matrix.value = m;

  return result;
}

static union data call_addition(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "Function %s expects 2 arguments\n", node->value.id.name);
    return result;
  }

  union data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i]);
    if (m[i].common.type != DATA_MATRIX) {
      fprintf(stderr, "In function %s, argument %d is not of type matrix.\n",
          node->value.id.name, i);
      return result;
    }
  }

  if (m[0].matrix.value->nbrows != m[1].matrix.value->nbrows
      && m[0].matrix.value->nbcols != m[1].matrix.value->nbcols) {
    fprintf(stderr, "In function %s, the matrices are not the same size.\n",
        node->value.id.name);
  }

  result.matrix.type = DATA_MATRIX;
  result.matrix.value = addition(m[0].matrix.value, m[1].matrix.value);

  deleteMatrix(m[0].matrix.value);
  deleteMatrix(m[1].matrix.value);

  return result;
}

static union data call_sub(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "Function %s expects 2 arguments\n", node->value.id.name);
    return result;
  }

  union data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i]);
    if (m[i].common.type != DATA_MATRIX) {
      fprintf(stderr, "In function %s, arguments are not all of type matrix.\n",
          node->value.id.name);
      return result;
    }
  }

  if (m[0].matrix.value->nbrows != m[1].matrix.value->nbrows
      && m[0].matrix.value->nbcols != m[1].matrix.value->nbcols) {
    fprintf(stderr, "In function %s, the matrices are not the same size.\n",
        node->value.id.name);
  }

  result.matrix.type = DATA_MATRIX;
  result.matrix.value = sub(m[0].matrix.value, m[1].matrix.value);

  deleteMatrix(m[0].matrix.value);
  deleteMatrix(m[1].matrix.value);

  return result;
}

static union data call_mult(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "Function %s expects 2 arguments\n", node->value.id.name);
    return result;
  }

  union data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i]);
    if (m[i].common.type != DATA_MATRIX) {
      fprintf(stderr, "In function %s, arguments are not all of type matrix.\n",
          node->value.id.name);
      return result;
    }
  }

  if (m[0].matrix.value->nbcols != m[1].matrix.value->nbrows) {
    fprintf(stderr, "In function %s, the matrices cannot be multiplied together.\n",
        node->value.id.name);
  }

  result.matrix.type = DATA_MATRIX;
  result.matrix.value = mult(m[0].matrix.value, m[1].matrix.value);

  deleteMatrix(m[0].matrix.value);
  deleteMatrix(m[1].matrix.value);

  return result;
}

static union data call_mult_scal(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "Function %s expects 2 arguments\n", node->value.id.name);
    return result;
  }

  union data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i]);
  }
  if (m[0].common.type != DATA_MATRIX) {
    fprintf(stderr, "In function %s, argument 0 is not of type matrix.\n",
        node->value.id.name);
    return result;
  }
  if (m[1].common.type != DATA_NUMBER) {
    fprintf(stderr, "In function %s, argument 1 is not of type number.\n",
        node->value.id.name);
    return result;
  }

  result.matrix.type = DATA_MATRIX;
  result.matrix.value = mult_scal(m[0].matrix.value, m[1].number.value);

  deleteMatrix(m[0].matrix.value);

  return result;
}

static union data call_expo(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "Function %s expects 2 arguments\n", node->value.id.name);
    return result;
  }

  union data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i]);
  }
  if (m[0].common.type != DATA_MATRIX) {
    fprintf(stderr, "In function %s, argument 0 is not of type matrix.\n",
        node->value.id.name);
    return result;
  }
  if (m[1].common.type != DATA_NUMBER) {
    fprintf(stderr, "In function %s, argument 1 is not of type number.\n",
        node->value.id.name);
    return result;
  }

  result.matrix.type = DATA_MATRIX;
  result.matrix.value = expo(m[0].matrix.value, m[1].number.value);

  deleteMatrix(m[0].matrix.value);

  return result;
}

static union data call_transpose(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1) {
    fprintf(stderr, "Function %s expects 1 arguments\n", node->value.id.name);
    return result;
  }

  union data m;
  m = extract_data(node->child[0]);
  if (m.common.type != DATA_MATRIX) {
    fprintf(stderr, "In function %s, argument 0 is not of type matrix.\n",
        node->value.id.name);
    return result;
  }

  result.matrix.type = DATA_MATRIX;
  result.matrix.value = transpose(m.matrix.value);

  deleteMatrix(m.matrix.value);

  return result;
}

static union data call_determinant(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1) {
    fprintf(stderr, "Function %s expects 1 arguments\n", node->value.id.name);
    return result;
  }

  union data m;
  m = extract_data(node->child[0]);
  if (m.common.type != DATA_MATRIX) {
    fprintf(stderr, "In function %s, argument 0 is not of type matrix.\n",
        node->value.id.name);
    return result;
  }

  if (m.matrix.value->nbrows != m.matrix.value->nbcols) {
    fprintf(stderr, "In function %s, argument 0 is not a square matrix.\n",
        node->value.id.name);
    return result;
  }

  result.number.type = DATA_NUMBER;
  result.number.value = determinant(m.matrix.value);

  deleteMatrix(m.matrix.value);

  return result;
}

static union data call_invert(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1) {
    fprintf(stderr, "Function %s expects 1 arguments\n", node->value.id.name);
    return result;
  }

  union data m;
  m = extract_data(node->child[0]);
  if (m.common.type != DATA_MATRIX) {
    fprintf(stderr, "In function %s, argument 0 is not of type matrix.\n",
        node->value.id.name);
    return result;
  }

  if (m.matrix.value->nbrows != m.matrix.value->nbcols) {
    fprintf(stderr, "In function %s, argument 0 is not a square matrix.\n",
        node->value.id.name);
    return result;
  }

  result.matrix.type = DATA_NUMBER;
  result.matrix.value = invert(m.matrix.value);

  deleteMatrix(m.matrix.value);

  return result;
}

static union data call_solve(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "Function %s expects 2 arguments\n", node->value.id.name);
    return result;
  }

  union data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i]);
    if (m[i].common.type != DATA_MATRIX) {
      fprintf(stderr, "In function %s, arguments are not all of type matrix.\n",
          node->value.id.name);
      return result;
    }
  }

  Matrix X = newMatrix(m[1].matrix.value->nbrows, m[1].matrix.value->nbcols);
  result.matrix.type = DATA_MATRIX;
  result.matrix.value = solve(m[0].matrix.value, m[1].matrix.value, X); //Won't work

  deleteMatrix(m[0].matrix.value);
  deleteMatrix(m[1].matrix.value);

  return result;
}

static union data call_rank(Tree node)
{
  union data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1) {
    fprintf(stderr, "Function %s expects 1 arguments\n", node->value.id.name);
    return result;
  }

  union data m;
  m = extract_data(node->child[0]);
  if (m.common.type != DATA_MATRIX) {
    fprintf(stderr, "In function %s, argument 0 is not of type matrix.\n",
        node->value.id.name);
    return result;
  }

  result.number.type = DATA_NUMBER;
  result.number.value = rank(m.matrix.value);

  deleteMatrix(m.matrix.value);

  return result;
}
