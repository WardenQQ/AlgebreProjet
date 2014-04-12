#include "operation.h"
#include "matrix.h"


int main()
{
  Matrix A = newMatrix(5,2);
  Matrix X;

  setElt(A,0,0,1);
  setElt(A,1,0,2);
  setElt(A,2,0,3);
  setElt(A,3,0,4);
  setElt(A,4,0,5);
  setElt(A,0,1,2);
  setElt(A,1,1,2);
  setElt(A,2,1,3);
  setElt(A,3,1,2);
  setElt(A,4,1,4);
  
  displayMatrix(A);

  X = least_estimate(A,"./least");

  displayMatrix(X);

  deleteMatrix(A);
  deleteMatrix(X);  
  return 0;
}
