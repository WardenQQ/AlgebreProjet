#include "matrix.h"
#include "operation.h"


int main()
{
  Matrix a = newMatrix(3,3);

  a = aleatoire(a,0,100);

  printf("det = %f\n",determinant(a));
  
  deleteMatrix(a);
  return 0;
}
