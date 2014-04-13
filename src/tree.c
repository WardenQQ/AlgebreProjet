#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

Tree newTree()
{
  Tree t = malloc(sizeof(*t));
  if (t == NULL) {
    perror("newTree() ");
    exit(1);
  }

  t->child = NULL;
  t->count = 0;
  t->size = 0;
  t->token.type = TOK_NONE;

  return t;
}

// TODO: remove this function.
void setValue(Tree t, Token value)
{
  t->token = value;
}

void setNbChild(Tree t, int nb)
{
  // On libère des struct tree si on en a trop.
  for (int  i = nb; i < t->count; ++i) {
    deleteTree(t->child[i]);
  }

  t->child = realloc(t->child, sizeof(*(t->child)) * nb);
  t->size = nb;
}

void addChild(Tree parent, Tree child)
{
  if (parent->count >= parent->size) {
    setNbChild(parent, parent->size + 1);
  }

  parent->child[parent->count] = child;
  ++parent->count;
}

void deleteTree(Tree t)
{
  for (int i = 0; i < t->count; ++i) {
    deleteTree(t->child[i]);
  }
  free(t->child);
  free(t);
}
