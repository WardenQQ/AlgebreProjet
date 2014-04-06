#ifndef _TREE_H_
#define _TREE_H_

#include <stddef.h>

#include "lexical_analyzer.h"

typedef struct tree {
  size_t count;
  size_t size;
  struct tree **child;
  union token value;
} *Tree;

Tree newTree(int nb_child);
void setValue(Tree t, union token value);
void addChild(Tree parent, Tree child);
void deleteTree(Tree t);

#endif //_TREE_H_
