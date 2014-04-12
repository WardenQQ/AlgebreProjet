#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef double E;

typedef struct matrix {
  E *mat;
  int nbrows,nbcols;
} matrix,* Matrix;

Matrix newMatrix(int nb_rows, int nb_columns);
E getElt(Matrix m, int rows, int columns);
void setElt(Matrix m, int row, int column, E val);
void deleteMatrix(Matrix m);
void displayMatrix(Matrix m);
Matrix identite(int nb_rows, int nb_columns);
Matrix aleatoire(int nb_rows, int nb_cols, E min, E max);

#endif
