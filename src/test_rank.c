#include <stdio.h>

#include "operation.h"
#include "matrix.h"

int main()
{
  Matrix a = newMatrix(4,3);

  setElt(a,0,0,1);
  setElt(a,0,1,1);
  setElt(a,0,2,1);

  setElt(a,1,0,1);
  setElt(a,1,1,2);
  setElt(a,1,2,1);

  setElt(a,2,0,1);
  setElt(a,2,1,1);
  setElt(a,2,2,3);

  setElt(a,3,0,1);
  setElt(a,3,1,3);
  setElt(a,3,2,1);
  printf("rang = %d\n",rank(a));

  deleteMatrix(a);

  return 0;
}
