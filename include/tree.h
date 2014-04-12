#ifndef _TREE_H_
#define _TREE_H_

#include <stddef.h>

#include "tokens.h"

typedef struct tree {
  size_t count;
  size_t size;
  struct tree **child;
  Token token;
} *Tree;

Tree newTree();
void setValue(Tree t, Token value);
void setNbChild(Tree t, size_t nb);
void addChild(Tree parent, Tree child);
void deleteTree(Tree t);

#endif //_TREE_H_
