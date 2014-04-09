#include "matrix.h"
#include "operation.h"


int main()
{
  Matrix m = newMatrix(5,2);
  m = aleatoire(m,0,10);
  displayMatrix(m);
  displayMatrix(transpose(m)); 


  return 0;
}
