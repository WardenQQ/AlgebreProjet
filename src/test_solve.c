#include "operation.h"
#include "matrix.h"

int main()
{
  Matrix a = newMatrix(3,3);
  
  a = aleatoire(a,0,100);

  displayMatrix(a);
  
  deleteMatrix(a);

   








  return 0;
}
