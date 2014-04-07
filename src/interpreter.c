#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

static union data extract_data(Tree node);
static union data call_function(Tree node);
static union data call_matrix(Tree node);
static union data call_addition(Tree node);

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
  } else if (strncmp(node->value.id.name, "mult", STRING_MAX) == 0) {
  } else if (strncmp(node->value.id.name, "mult_scal", STRING_MAX) == 0) {
  } else if (strncmp(node->value.id.name, "expo", STRING_MAX) == 0) {
  } else if (strncmp(node->value.id.name, "transpose", STRING_MAX) == 0) {
  } else if (strncmp(node->value.id.name, "determinant", STRING_MAX) == 0) {
  } else if (strncmp(node->value.id.name, "invert", STRING_MAX) == 0) {
  } else if (strncmp(node->value.id.name, "solve", STRING_MAX) == 0) {
  } else if (strncmp(node->value.id.name, "rank", STRING_MAX) == 0) {
  } else if (strncmp(node->value.id.name, "quit", STRING_MAX) == 0) {
    exit(0);
  } else {
    fprintf(stderr, "%s is not a function\n", node->value.id.name);
  }

  return data;
}

static union data call_matrix(Tree node)
{
  union data mat;
  mat.common.type = DATA_NULL;

  int nb_rows = node->count;
  for (int i = 0; i < nb_rows; ++i) {
    if (node->child[i]->value.type != TOK_VECTOR) {
      fprintf(stderr, "Expected a parameter of type list in function %s\n",
          node->value.id.name);
      return mat;
    }
  }
  int nb_columns = node->child[0]->count;

  Matrix m = newMatrix(nb_rows, nb_columns);

  for (int i = 0; i < nb_rows; ++i) {
    Tree list = node->child[i];
    for (int j = 0; j < nb_columns; ++j) {
      union data element = extract_data(list->child[j]);
      if (element.common.type != DATA_NUMBER) {
        fprintf(stderr, "List contains a value which is not a number\n");
        return mat;
      }
      setElt(m, i, j, element.number.value);
    }
  }

  mat.matrix.type = DATA_MATRIX;
  mat.matrix.value = m;

  return mat;
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

  result.matrix.type = DATA_MATRIX;
  result.matrix.value = addition(m[0].matrix.value, m[1].matrix.value);

  return result;
}
