#include "matrix.h"
#include "operation.h"

int main()
{
  Matrix a = newMatrix(3,3);

  a = aleatoire(a,0,100);

  displayMatrix(a);

  Matrix m = invert(a);

  displayMatrix(a);

  displayMatrix(m);

  deleteMatrix(a);

  deleteMatrix(m);


  return 0;
}
