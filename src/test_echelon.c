#include "operation.h"
#include "matrix.h"

int main()
{
  Matrix A = aleatoire(6,6,-10,10);
  Matrix B = identite(6,6);
  Matrix I = copie_matrix(A);

  reduced_row_echelon_form(I, B);

  displayMatrix(mult(A,B));
}
