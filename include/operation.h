#ifndef __OPERATION_H_
#define __OPERATION_H_
#include "matrix.h"


Matrix addition(Matrix m1,Matrix m2);

Matrix mult(Matrix m1,Matrix m2);

Matrix sub(Matrix m1,Matrix m2);

Matrix transpose(Matrix m);

Matrix mult_scal(Matrix m,E s);

E valeur_absolue(E e);

int choixPivot(Matrix m,int i);

void echangeLigne(Matrix m,int i,int j);

void addmultiple(Matrix m,int i,int j,E c);

E triangulaire_det(Matrix m);

E determinant(Matrix m);

Matrix expo(Matri m,int exposant);

E puisscom(E e,int i,int j);

Matrix extraction(Matrix m,int i,int j);

Matrix comatrice(Matrix m);

Matrix invert(Matrix m);

#endif
