#include <stdio.h>

#include "matrix.h"
#include "operation.h"

int main()
{

  Matrix m = aleatoire(3, 3, 0, 100);

  printf("plus grande valeur propre : %f\n",valeur_propre(m,0.003));

  deleteMatrix(m);

  return 0;
}
