#ifndef __OPERATION_H_
#define __OPERATION_H_

#include "matrix.h"

Matrix copie_matrix(Matrix m);
Matrix addition(Matrix m1, Matrix m2);
Matrix sub(Matrix m1, Matrix m2);
Matrix mult(Matrix m1, Matrix m2);
Matrix mult_scal(Matrix m, E s);
Matrix expo(Matrix m, int exposant);
Matrix transpose(Matrix m);
int determinant(Matrix m, E *det);
Matrix invert(Matrix m);
Matrix solve(Matrix A, Matrix B);
int rank(Matrix A);
Matrix least_estimate(Matrix A, char * nom_fichier);
E valeur_propre(Matrix A,E precision);

#endif
