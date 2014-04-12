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
E determinant(Matrix m);
Matrix invert(Matrix m);
Matrix solve(Matrix A,Matrix B);
int rank(Matrix A);
Matrix least_estimate(Matrix A, char * nom_fichier);

E reduced_row_echelon_form(Matrix A, Matrix B);

void echangeLigne_det(Matrix m,int i,int j);

void echangeLigne(Matrix A,Matrix B,int i,int j);

void addmultiple_det(Matrix m,int i,int j,E c);

E triangulaire_det(Matrix m);



Matrix extraction(Matrix m,int i,int j);

Matrix comatrice(Matrix m);


void remontee(Matrix A,Matrix B,Matrix X);

void triangulaire(Matrix A,Matrix B);


void addmultiple(Matrix A,Matrix B,int i,int j,E c);

int puisscom(int i,int j);


int plus_grande_compo(Matrix A);

E plus_grande_compo_absolue(Matrix A);

E valeur_propre(Matrix A,E precision);

void speedtest(char * commande, int taille_min, int taille_max, int pas, int nb_sec);


#endif
