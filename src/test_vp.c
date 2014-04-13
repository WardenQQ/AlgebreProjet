#include <stdio.h>

#include "matrix.h"
#include "operation.h"

int main()
{

  Matrix m = aleatoire(3, 3, 0, 100);
  eigenvalue_t e = eigenvalues(m,0.01);
  displayMatrix(e.vecteur_propre);

  deleteMatrix(m);

  return 0;
}
