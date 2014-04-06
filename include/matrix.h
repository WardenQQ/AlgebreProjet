#ifndef __MATRIX_H_
#define __MATRIX_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef float E;

typedef struct matrix {
	E *mat;
	int nbrows,nbcols;

} matrix,* Matrix;

Matrix newMatrix(int nb_rows,int nb_columns);

E getElt(Matrix m,int rows,int columns);

void setElt(Matrix m,int row,int column,E val);

void deleteMatrix(Matrix m);

void displayMatrix(Matrix m);

Matrix identite(int nb_rows,int nb_columns);

Matrix aleatoire(Matrix m,E min,E max);

Matrix copie_matrix(Matrix m);

#endif
