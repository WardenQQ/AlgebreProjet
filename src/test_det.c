#include <stdio.h>

#include "matrix.h"
#include "operation.h"


int main()
{
  Matrix a = aleatoire(3, 3, 0, 100);

  printf("det = %f\n", determinant(a));
  
  deleteMatrix(a);

  return 0;
}
