#include <assert.h>
#include "tree.h"

int main()
{
  Tree t = newTree(8);

  for (int i = 0; i < 8; ++i) {
    addChild(t, newTree(4));
  }
  assert(t->count == 8);

  deleteTree(t);
}
