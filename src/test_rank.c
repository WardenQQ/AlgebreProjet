#include <stdio.h>

#include "operation.h"
#include "matrix.h"

int main()
{
  Matrix a = aleatoire(3, 3, 0, 100);

  printf("rang = %d\n",rank(a));

  deleteMatrix(a);

  return 0;
}
