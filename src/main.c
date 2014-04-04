#include "matrix.h"
#include "operation.h"



int main(int argc,char * argv[])
{
  Matrix A = newMatrix(3,3);
  
  Matrix X = newMatrix(3,1);

  Matrix B = newMatrix(3,1);

  A = aleatoire(A,0,5); 

  B = aleatoire(B,0,5);
  
  //solve(A,B,X);

  displayMatrix(A);

  //displayMatrix(B);

  //displayMatrix(X);

  //printf("le determinant est : %f\n",determinant(A));
  printf("le rang de la matrice est : %d\n",rank(A));
  //Matrix mi;
  
  //mi = invert(A); 

  //displayMatrix(mi);
  
  deleteMatrix(A);
  deleteMatrix(B);
  deleteMatrix(X);
  return 0;
}
