#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"
#include "speedtest.h"

static Data extract_data(Tree node, SymbolTable symbol_table);

static Data assign(Tree node, SymbolTable symbol_table);

static Data call_function(Tree node, SymbolTable symbol_table);
static Data call_matrix(Tree node, SymbolTable symbol_table);
static Data call_addition(Tree node, SymbolTable symbol_table);
static Data call_sub(Tree node, SymbolTable symbol_table);
static Data call_mult(Tree node, SymbolTable symbol_table);
static Data call_mult_scal(Tree node, SymbolTable symbol_table);
static Data call_expo(Tree node, SymbolTable symbol_table);
static Data call_transpose(Tree node, SymbolTable symbol_table);
static Data call_determinant(Tree node, SymbolTable symbol_table);
static Data call_invert(Tree node, SymbolTable symbol_table);
static Data call_solve(Tree node, SymbolTable symbol_table);
static Data call_rank(Tree node, SymbolTable symbol_table);
static Data call_speedtest(Tree node, SymbolTable symbol_table);
static Data call_eigenvalues(Tree node, SymbolTable symbol_table);
static Data call_least_estimate(Tree node, SymbolTable symbol_table);


void interpreter(Tree root, SymbolTable symbol_table)
{
  Data data = extract_data(root, symbol_table);

  switch (data.common.type) {
    case DATA_NUMBER:
      printf("    %- 8.2f\n", data.number.value);
      break;
    case DATA_MATRIX:
      displayMatrix(data.matrix.value);
      if (data.matrix.is_temp) {
        deleteMatrix(data.matrix.value);
      }
      break;
    default:
      break;
  }
}

static Data extract_data(Tree node, SymbolTable symbol_table)
{
  Data data = {.common = {.type = DATA_NULL}};

  switch (node->token.type) {
    case TOK_COLON:
      data = assign(node, symbol_table);
      break;
    case TOK_FUNCTION:
      data = call_function(node, symbol_table);
      break;
    case TOK_ID:
      data = find_entry_data(symbol_table, node->token.id.name);
      break;
    case TOK_NUMBER:
      data.number.type = DATA_NUMBER;
      data.number.value = node->token.number.value;
      break;
    case TOK_LIST:
      //TODO: message erreur.
      break;
    default:
      break;
  }

  return data;
}

static Data assign(Tree node, SymbolTable symbol_table)
{
  Data data = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "L'opérateur ':' est un opérateur binaire.\n");
  }

  data = extract_data(node->child[1], symbol_table);
  if (data.common.type == DATA_MATRIX) {
    data.matrix.is_temp = false;
  }
  add_entry(symbol_table, node->child[0]->token.id.name, data);

  return data;
}

static Data call_function(Tree node, SymbolTable symbol_table)
{
  Data data;
  if (strncmp(node->token.id.name, "matrix", STRING_MAX) == 0) {
    data = call_matrix(node, symbol_table);
  } else if (strncmp(node->token.id.name, "addition", STRING_MAX) == 0) {
    data = call_addition(node, symbol_table);
  } else if (strncmp(node->token.id.name, "sub", STRING_MAX) == 0) {
    data = call_sub(node, symbol_table);
  } else if (strncmp(node->token.id.name, "mult", STRING_MAX) == 0) {
    data = call_mult(node, symbol_table);
  } else if (strncmp(node->token.id.name, "mult_scal", STRING_MAX) == 0) {
    data = call_mult_scal(node, symbol_table);
  } else if (strncmp(node->token.id.name, "expo", STRING_MAX) == 0) {
    data = call_expo(node, symbol_table);
  } else if (strncmp(node->token.id.name, "transpose", STRING_MAX) == 0) {
    data = call_transpose(node, symbol_table);
  } else if (strncmp(node->token.id.name, "determinant", STRING_MAX) == 0) {
    data = call_determinant(node, symbol_table);
  } else if (strncmp(node->token.id.name, "invert", STRING_MAX) == 0) {
    data = call_invert(node, symbol_table);
  } else if (strncmp(node->token.id.name, "solve", STRING_MAX) == 0) {
    data = call_solve(node, symbol_table);
  } else if (strncmp(node->token.id.name, "rank", STRING_MAX) == 0) {
    data = call_rank(node, symbol_table);
  } else if (strncmp(node->token.id.name, "speedtest", STRING_MAX) == 0) {
    data = call_speedtest(node, symbol_table);
  } else if (strncmp(node->token.id.name, "eigenvalue", STRING_MAX) == 0) {
    data = call_eigenvalues(node, symbol_table);
  } else if (strncmp(node->token.id.name, "least_estimate", STRING_MAX) == 0) {
    data = call_least_estimate(node, symbol_table);
  } else if (strncmp(node->token.id.name, "quit", STRING_MAX) == 0) {
    exit(0);
  } else {
    fprintf(stderr, "%s n'est pas un fonction.\n", node->token.id.name);
  }

  return data;
}

static Data call_matrix(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count < 1) {
    fprintf(stderr, "La fonction %s s'attend à au moins un argument.\n",
        node->token.id.name);
    return result;
  }

  int nb_rows = node->count;
  int nb_columns = node->child[0]->count;

  Matrix m = newMatrix(nb_rows, nb_columns);
  if (m == NULL) {
    return result;
  }

  for (int i = 0; i < nb_rows; ++i) {
    Tree list = node->child[i];
    if (list->token.type != TOK_LIST) {
      fprintf(stderr, "Dans la fonction %s l'argument n'est pas de type listes\n",
          node->token.id.name);
      return result;
    }

    if (list->count != nb_columns) {
      fprintf(stderr,
          "Dans la fonction %s"
          " les listes passés en argument doivent etre de meme taille.\n",
          node->token.id.name);
      return result;
    }

    for (int j = 0; j < nb_columns; ++j) {
      Data element = extract_data(list->child[j], symbol_table);
      if (element.common.type != DATA_NUMBER) {
        fprintf(stderr, "Les elements de la liste doivent etre du type nombre.\n");
        return result;
      }
      setElt(m, i, j, element.number.value);
    }
  }

  result.matrix.value = m;
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  return result;
}

static Data call_addition(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "La fonction %s s'attend à 2 arguments.\n", node->token.id.name);
    return result;
  }

  Data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i], symbol_table);
    if (m[i].common.type != DATA_MATRIX) {
      fprintf(stderr,
          "Dans la fonction %s l'argument %d n'est pas de type matrice.\n",
          node->token.id.name, i + 1);
      return result;
    }
  }

  result.matrix.value = addition(m[0].matrix.value, m[1].matrix.value);
  if (result.matrix.value == NULL) {
    return result;
  }
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  if (m[0].matrix.is_temp) {
    deleteMatrix(m[0].matrix.value);
  }
  if (m[1].matrix.is_temp) {
    deleteMatrix(m[1].matrix.value);
  }

  return result;
}

static Data call_sub(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "La fonction %s s'attend à 2 arguments.\n", node->token.id.name);
    return result;
  }

  Data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i], symbol_table);
    if (m[i].common.type != DATA_MATRIX) {
      fprintf(stderr,
          "Dans la fonction %s l'argument %d n'est pas de type matrice.\n",
          node->token.id.name, i + 1);
      return result;
    }
  }

  result.matrix.value = sub(m[0].matrix.value, m[1].matrix.value);
  if (result.matrix.value == NULL) {
    return result;
  }
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  if (m[0].matrix.is_temp) {
    deleteMatrix(m[0].matrix.value);
  }
  if (m[1].matrix.is_temp) {
    deleteMatrix(m[1].matrix.value);
  }

  return result;
}

static Data call_mult(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "La fonction %s s'attend à 2 arguments.\n", node->token.id.name);
    return result;
  }

  Data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i], symbol_table);
    if (m[i].common.type != DATA_MATRIX) {
      fprintf(stderr,
          "Dans la fonction %s l'argument %d n'est pas de type matrice.\n",
          node->token.id.name, i + 1);
      return result;
    }
  }

  result.matrix.value = mult(m[0].matrix.value, m[1].matrix.value);
  if (result.matrix.value == NULL) {
    return result;
  }
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  if (m[0].matrix.is_temp) {
    deleteMatrix(m[0].matrix.value);
  }
  if (m[1].matrix.is_temp) {
    deleteMatrix(m[1].matrix.value);
  }

  return result;
}

static Data call_mult_scal(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "La fonction %s s'attend à 2 arguments.\n", node->token.id.name);
    return result;
  }

  Data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i], symbol_table);
  }
  if (m[0].common.type != DATA_MATRIX) {
    fprintf(stderr, "Dans la fonction %s, l'argument 1 n'est pas du type matrice.\n",
        node->token.id.name);
    return result;
  }
  if (m[1].common.type != DATA_NUMBER) {
    fprintf(stderr, "Dans la fonction %s, l'argument 2 n'est pas du type nombre.\n",
        node->token.id.name);
    return result;
  }

  result.matrix.value = mult_scal(m[0].matrix.value, m[1].number.value);
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  if (m[0].matrix.is_temp) {
    deleteMatrix(m[0].matrix.value);
  }

  return result;
}

static Data call_expo(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "La fonction %s s'attend à 2 arguments.\n", node->token.id.name);
    return result;
  }

  Data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i], symbol_table);
  }
  if (m[0].common.type != DATA_MATRIX) {
    fprintf(stderr, "Dans la fonction %s, l'argument 1 n'est pas du type matrice.\n",
        node->token.id.name);
    return result;
  }
  if (m[1].common.type != DATA_NUMBER) {
    fprintf(stderr, "Dans la fonction %s, l'argument 2 n'est pas du type nombre.\n",
        node->token.id.name);
    return result;
  }

  result.matrix.value = expo(m[0].matrix.value, m[1].number.value);
  if (result.matrix.value == NULL) {
    return result;
  }
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  if (m[0].matrix.is_temp) {
    deleteMatrix(m[0].matrix.value);
  }

  return result;
}

static Data call_transpose(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1) {
    fprintf(stderr, "La fonction %s s'attend à 1 arguments.\n", node->token.id.name);
    return result;
  }

  Data m;
  m = extract_data(node->child[0], symbol_table);
  if (m.common.type != DATA_MATRIX) {
    fprintf(stderr, "Dans la fonction %s, l'argument 1 n'est pas du type matrice.\n",
        node->token.id.name);
    return result;
  }

  result.matrix.value = transpose(m.matrix.value);
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  if (m.matrix.is_temp) {
    deleteMatrix(m.matrix.value);
  }

  return result;
}

static Data call_determinant(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1) {
    fprintf(stderr, "La fonction %s s'attend à 1 arguments.\n", node->token.id.name);
    return result;
  }

  Data m;
  m = extract_data(node->child[0], symbol_table);
  if (m.common.type != DATA_MATRIX) {
    fprintf(stderr, "Dans la fonction %s, l'argument 1 n'est pas du type matrice.\n",
        node->token.id.name);
    return result;
  }

  if (determinant(m.matrix.value, &result.number.value) == 0) {
    return result;
  }
  result.number.type = DATA_NUMBER;

  if (m.matrix.is_temp) {
    deleteMatrix(m.matrix.value);
  }

  return result;
}

static Data call_invert(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1) {
    fprintf(stderr, "Function %s expects 1 arguments\n", node->token.id.name);
    return result;
  }

  Data m;
  m = extract_data(node->child[0], symbol_table);
  if (m.common.type != DATA_MATRIX) {
    fprintf(stderr, "In function %s, argument 0 is not of type matrix.\n",
        node->token.id.name);
    return result;
  }

  result.matrix.value = invert(m.matrix.value);
  if (result.matrix.value == NULL) {
    return result;
  }
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  if (m.matrix.is_temp) {
    deleteMatrix(m.matrix.value);
  }

  return result;
}

static Data call_solve(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 2) {
    fprintf(stderr, "La fonction %s s'attend à 2 arguments.\n", node->token.id.name);
    return result;
  }

  Data m[2];
  for (int i = 0; i < 2; ++i) {
    m[i] = extract_data(node->child[i], symbol_table);
    if (m[i].common.type != DATA_MATRIX) {
      fprintf(stderr,
          "Dans la fonction %s l'argument %d n'est pas de type matrice.\n",
          node->token.id.name, i + 1);
      return result;
    }
  }

  result.matrix.value = solve(m[0].matrix.value, m[1].matrix.value);
  if (result.matrix.value == NULL) {
    return result;
  }
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  if (m[0].matrix.is_temp) {
    deleteMatrix(m[0].matrix.value);
  }
  if (m[1].matrix.is_temp) {
    deleteMatrix(m[1].matrix.value);
  }

  return result;
}

static Data call_rank(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1) {
    fprintf(stderr, "La fonction %s s'attend à 1 arguments.\n", node->token.id.name);
    return result;
  }

  Data m;
  m = extract_data(node->child[0], symbol_table);
  if (m.common.type != DATA_MATRIX) {
    fprintf(stderr, "Dans la fonction %s, l'argument 1 n'est pas du type matrice.\n",
        node->token.id.name);
    return result;
  }

  result.number.value = rank(m.matrix.value);
  result.number.type = DATA_NUMBER;
  result.matrix.is_temp = true;

  if (m.matrix.is_temp) {
    deleteMatrix(m.matrix.value);
  }

  return result;
}

static Data call_speedtest(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 4 && node->count != 5) {
    fprintf(stderr, "La fonction %s s'attend à 4 ou 5 arguments.\n", node->token.id.name);
    return result;
  }

  if (node->child[0]->token.type != TOK_ID) {
    fprintf(stderr, "Dans la fonction %s, l'argument 1 n'est pas un string.\n",
        node->token.id.name);
  }
  Data option = {.common = {.type = DATA_NUMBER}};
  Data m[3];
  for (int i = 0; i < 3; ++i) {
    m[i] = extract_data(node->child[i + 1], symbol_table);
  }
  if (m[0].common.type != DATA_NUMBER) {
    fprintf(stderr, "Dans la fonction %s, l'argument 2 n'est pas du type nombre.\n",
        node->token.id.name);
    return result;
  }
  if (m[1].common.type != DATA_NUMBER) {
    fprintf(stderr, "Dans la fonction %s, l'argument 3 n'est pas du type nombre.\n",
        node->token.id.name);
    return result;
  }
  if (m[2].common.type != DATA_NUMBER) {
    fprintf(stderr, "Dans la fonction %s, l'argument 4 n'est pas du type nombre.\n",
        node->token.id.name);
    return result;
  }
  if (node->count == 5) {
    option = extract_data(node->child[4], symbol_table);
    if (option.common.type != DATA_NUMBER) {
      fprintf(stderr, "Dans la fonction %s, l'argument 5 n'est pas du type nombre.\n",
          node->token.id.name);
      return result;
    }
  } else {
    option.number.value = 0;
  }

  speedtest(node->child[0]->token.id.name, m[0].number.value, m[1].number.value,
      m[2].number.value, option.number.value);

  return result;
}


static Data call_eigenvalues(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1 && node->count != 2) {
    fprintf(stderr, "La fonction %s s'attend à 1 ou 2 arguments.\n", node->token.id.name);
    return result;
  }

  Data m = extract_data(node->child[0], symbol_table);
  if (m.common.type != DATA_MATRIX) {
      fprintf(stderr, "Dans la fonction %s, l'argument 1 n'est pas du type matrice.\n",
          node->token.id.name);
      return result;
  }

  Data option = {.common = {.type = DATA_NUMBER}};
  if (node->count == 2) {
    option = extract_data(node->child[1], symbol_table);
    if (option.common.type != DATA_NUMBER) {
      fprintf(stderr, "Dans la fonction %s, l'argument 2 n'est pas du type nombre.\n",
          node->token.id.name);
      return result;
    }
  } else {
    option.number.value = 0.01;
  }

  eigenvalue_t ev = eigenvalues(m.matrix.value, option.number.value);
  if (ev.vecteur_propre == NULL) {
    return result;
  }
  result.matrix.value = ev.vecteur_propre;
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  if (m.matrix.is_temp) {
    deleteMatrix(m.matrix.value);
  }

  printf("Plus grande valeur propre: %- 8.2f\n", ev.valeur_propre);

  return result;
}

static Data call_least_estimate(Tree node, SymbolTable symbol_table)
{
  Data result = {.common = {.type = DATA_NULL}};
  if (node->count != 1 && node->count != 2) {
    fprintf(stderr, "La fonction %s s'attend à 1 ou 2 arguments.\n", node->token.id.name);
    return result;
  }

  Data m = extract_data(node->child[0], symbol_table);
  if (m.common.type != DATA_MATRIX) {
      fprintf(stderr, "Dans la fonction %s, l'argument 1 n'est pas du type matrice.\n",
          node->token.id.name);
      return result;
  }

  char *name = NULL;
  if (node->count == 2) {
    if (node->child[1]->token.type != TOK_ID) {
      fprintf(stderr, "Dans la fonction %s, l'argument 2 n'est pas du type string.\n",
          node->token.id.name);
      return result;
    }
    name = node->child[1]->token.id.name;
  }

  least_squares_t ls = least_estimate(m.matrix.value, name);
  if (ls.coef_droite == NULL && ls.residu == NULL) {
    return result;
  }
  result.matrix.value = ls.coef_droite;
  result.matrix.type = DATA_MATRIX;
  result.matrix.is_temp = true;

  printf("Les residus:\n");
  displayMatrix(ls.residu);
  deleteMatrix(ls.residu);

  if (m.matrix.is_temp) {
    deleteMatrix(m.matrix.value);
  }

  return result;
}
