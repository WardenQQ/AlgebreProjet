#include "matrix.h"
#include "operation.h"


int main()
{
  Matrix m = aleatoire(5, 2, 0, 10);
  displayMatrix(m);
  displayMatrix(transpose(m)); 
  displayMatrix(least_estimate(m,"./dfshj"));
  deleteMatrix(m);

  return 0;
}
