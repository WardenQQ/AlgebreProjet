#ifndef _TREE_H_
#define _TREE_H_

#include "tokens.h"

typedef struct tree {
  int count;
  int size;
  struct tree **child;
  Token token;
} *Tree;

Tree newTree();
void setValue(Tree t, Token value);
void setNbChild(Tree t, int nb);
void addChild(Tree parent, Tree child);
void deleteTree(Tree t);

#endif //_TREE_H_
