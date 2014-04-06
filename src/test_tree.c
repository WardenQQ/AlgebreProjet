#include <assert.h>
#include "tree.h"

int main()
{
  Tree t = newTree();

  for (int i = 0; i < 8; ++i) {
    addChild(t, newTree());
  }
  assert(t->count == 8);

  deleteTree(t);
}
