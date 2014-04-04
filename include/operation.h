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

void echangeLigne_det(Matrix m,int i,int j);

void echangeLigne(Matrix A,Matrix B,int i,int j);

void addmultiple_det(Matrix m,int i,int j,E c);

E triangulaire_det(Matrix m);

E determinant(Matrix m);

Matrix expo(Matrix m,int exposant);

Matrix extraction(Matrix m,int i,int j);

Matrix comatrice(Matrix m);

Matrix invert(Matrix m);

void remontee(Matrix A,Matrix B,Matrix X);

void triangulaire(Matrix A,Matrix B);

Matrix PivotDeGauss(Matrix A,Matrix B,Matrix X);

void addmultiple(Matrix A,Matrix B,int i,int j,E c);

int puisscom(int i,int j);

#endif
