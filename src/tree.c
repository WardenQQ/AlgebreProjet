#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

Tree newTree(int nb_child)
{
  Tree t = malloc(sizeof(*t));
  if (t == NULL) {
    perror("newTree() ");
    exit(1);
  }

  t->child = malloc(sizeof(*(t->child)) * nb_child);
  if (t->child == NULL) {
    perror("newTree() ");
    exit(1);
  }

  t->count = 0;
  t->size = nb_child;
  t->value.type = TOK_NONE;

  return t;
}

void setValue(Tree t, union token value)
{
  t->value = value;
}

void addChild(Tree parent, Tree child)
{
  assert(parent->count < parent->size);

  parent->child[parent->count] = child;
  ++parent->count;
}

void deleteTree(Tree t)
{
  for (size_t i = 0; i < t->count; ++i) {
    deleteTree(t->child[i]);
  }
  free(t->child);
  free(t);
}
