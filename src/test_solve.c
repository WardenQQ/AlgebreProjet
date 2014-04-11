#include "operation.h"
#include "matrix.h"

int main()
{
  Matrix a = aleatoire(3, 3, 0, 100);

  displayMatrix(a);
  
  deleteMatrix(a);

  return 0;
}
