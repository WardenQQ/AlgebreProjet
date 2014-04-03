#include "matrix.h"
#include "operation.h"



int main(int argc,char * argv[])
{
  Matrix m = newMatrix(10,10);
  m = aleatoire(m,0,100); 

  displayMatrix(m);

  printf("le determinant est : %f\n",determinant(m));

  Matrix mi = newMatrix(10,10);
  
  mi = expo(m,5); 

  displayMatrix(mi);

  return 0;
}
