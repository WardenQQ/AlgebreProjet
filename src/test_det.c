#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "matrix.h"
#include "operation.h"

int main()
{
  Matrix A = newMatrix(3,3);
  setElt(A, 0, 0, 1);
  setElt(A, 0, 1, 2);
  setElt(A, 0, 2, -3);
  setElt(A, 1, 0, 4);
  setElt(A, 1, 1, 3);
  setElt(A, 1, 2, -1);
  setElt(A, 2, 0, 0);
  setElt(A, 2, 1, 1);
  setElt(A, 2, 2, 2);

  E det;
  determinant_bis(A, &det);
  deleteMatrix(A);
  assert(det == -21);
}
