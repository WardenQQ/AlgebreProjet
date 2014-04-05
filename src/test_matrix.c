#include "matrix.h"
#include "operation.h"



int main(int argc,char * argv[])
{
  Matrix m = newMatrix(3,3);
  setElt(m,0,0,1);
  setElt(m,0,1,-1);
  setElt(m,0,2,2);
  setElt(m,1,0,3);
  setElt(m,1,1,2);
  setElt(m,1,2,1);
  setElt(m,2,0,2);
  setElt(m,2,1,-3);
  setElt(m,2,2,-2);

  Matrix X = newMatrix(3,1);

  Matrix B = newMatrix(3,1);

  setElt(B,0,0,5);
  setElt(B,1,0,10);
  setElt(B,2,0,-10);
  
  PivotDeGauss(m,B,X);

  displayMatrix(m);

  displayMatrix(B);

  displayMatrix(X);

  //printf("le determinant est : %f\n",determinant(m));

  //Matrix mi = newMatrix(3,3);
  
  //mi = invert(m); 

  //displayMatrix(mi);

  return 0;
}
