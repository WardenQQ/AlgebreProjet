#include "matrix.h"
#include "operation.h"

int main()
{
  Matrix a = aleatoire(3, 3, 0, 100);

  displayMatrix(a);

  Matrix m = invert(a);

  displayMatrix(a);

  displayMatrix(m);

  deleteMatrix(a);

  deleteMatrix(m);


  return 0;
}
