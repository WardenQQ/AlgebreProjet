#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "limits.h"
#include "operation.h"

static E valeur_absolue(E e);
static int choose_pivot(Matrix m, int row, int col);
static void swap_line(Matrix m, int l1, int l2);
static void combine_line(Matrix m, E c1, int l1, E c2, int l2);
static E reduced_row_echelon_form(Matrix A, Matrix B);
static E plus_grande_compo_absolue(Matrix A);
static int plus_grande_compo(Matrix A);

Matrix copie_matrix(Matrix m)
{
  Matrix m_copie = newMatrix(m->nbrows, m->nbcols);

  for (int i = 0; i < m->nbrows; i++) {
    for (int j = 0; j < m->nbcols; j++) {
      setElt(m_copie, i, j, getElt(m, i, j));
    }
  }

  return m_copie;
}

Matrix addition(Matrix m1,Matrix m2)
{
  if ((m1->nbcols != m2->nbcols) || (m1->nbrows != m2->nbrows))	{
    fprintf(stderr, "La taille des deux matrices sont différentes,"
        " impossible de faire l'addition.\n");
    return NULL;
  }

  Matrix m_add = newMatrix(m1->nbrows, m1->nbcols);
  for (int i = 0; i < m1->nbrows; i++) {
    for (int j = 0; j < m1->nbcols; j++) {
      setElt(m_add, i, j, getElt(m1, i, j) + getElt(m2, i, j));
    }
  }

  return m_add;
}

Matrix sub(Matrix m1, Matrix m2)
{
  if ((m1->nbcols != m2->nbcols) || (m1->nbrows != m2->nbrows))	{
    fprintf(stderr, "La taille des deux matrices sont différentes,"
        " impossible de faire la substraction.\n");
    return NULL;
  }

  Matrix m_sub = newMatrix(m1->nbrows, m1->nbcols);
  for(int i = 0; i < m1->nbrows; i++) {
    for(int j = 0; j < m1->nbcols; j++) {
      setElt(m_sub, i, j, getElt(m1, i, j) - getElt(m2, i, j));
    }
  }

  return m_sub;
}

Matrix mult(Matrix m1, Matrix m2)
{
  if (m1->nbcols != m2->nbrows) {
    fprintf(stderr, "Le nombre de colonnes de la matrice 1 est différent"
        " du nombre de lignes de la matrice 2,"
        " impossible de faire la multiplication.\n");
    return NULL;
  }
	
  Matrix m_mult = newMatrix(m1->nbrows, m2->nbcols);

  for(int i = 0; i < m1->nbrows; i++) {
    for(int j = 0; j < m2->nbcols; j++) {
    E mult = 0;
      for(int k = 0; k < m1->nbcols; k++) {
        mult = mult + (getElt(m1, i, k) * getElt(m2, k, j));
      }	
    setElt(m_mult, i, j, mult);
    }
  }

  return m_mult;
}

Matrix mult_scal(Matrix m, E s)
{	
  Matrix m_mult = newMatrix(m->nbrows, m->nbcols);  

  for(int i = 0; i < m->nbrows; i++) {
    for(int j = 0; j < m->nbcols; j++) {
      setElt(m_mult, i, j, getElt(m, i, j) * s);
    }
  }

  return m_mult;
}

Matrix expo(Matrix m, int exposant)
{
  if (exposant < 1) {
    fprintf(stderr, "L'exposant doit etre supérieur à 1.");
    return NULL;
  }

  Matrix m_expo = copie_matrix(m);
  for(; exposant>1; exposant--)
  {
    Matrix tmp = m_expo;
    m_expo = mult(m_expo,m);
    deleteMatrix(tmp);
  }
  
  return m_expo; 
}

Matrix transpose(Matrix m)
{
  Matrix m_t = newMatrix(m->nbcols, m->nbrows);
  for(int i = 0; i < m->nbrows; i++)
  {
    for(int j = 0; j < m->nbcols; j++)
    {
      setElt(m_t, j, i, getElt(m, i, j));
    }
  }
 
  return m_t;
}

int determinant(Matrix m, E *det)
{
  if (m->nbcols != m->nbrows) {
    fprintf(stderr, "La matrice doit etre carrée.\n");
    return 0;
  }

  Matrix m_copie = copie_matrix(m);
  *det = reduced_row_echelon_form(m_copie, NULL);

  deleteMatrix(m_copie);

  return 1;
}

Matrix invert(Matrix m)
{
  if (m->nbrows != m->nbcols) {
    fprintf(stderr, "La matrice n'est pas une matrice carré.\n");
    return NULL;
  }
  Matrix m_inverse = identite(m->nbrows, m->nbcols);
  Matrix m_tmp = copie_matrix(m);

  E det = reduced_row_echelon_form(m_tmp, m_inverse);
  deleteMatrix(m_tmp);
  if (det == 0.0) {
    fprintf(stderr, "La matrice n'est pas inversible.\n");
    return NULL;
  }

  return m_inverse;
}

Matrix solve(Matrix A, Matrix B)
{
  if (B->nbcols != 1) {
    fprintf(stderr, "L'argument 2 n'est pas un vecteur colonne.\n");
    return NULL;
  }
  if (A->nbrows != A->nbcols) {
    fprintf(stderr, "L'argument 1 n'est pas une matrice carré.\n");
    return NULL;
  }
  if (B->nbrows != A->nbcols) {
    fprintf(stderr, "Le nombre de colonnes de la matrice de l'argument 1 est"
        " différent du nombre de ligne de la matrice de l'argument 2.\n");
    return NULL;
  }

  Matrix I = copie_matrix(A);
  Matrix X = copie_matrix(B);

  E det = reduced_row_echelon_form(I, X);
  deleteMatrix(I);
  if (det == 0.0) {
    fprintf(stderr, "La matrice de l'argument 1 a un determinant nul.\n");
    return NULL;
  }

  return X;
}

int rank(Matrix m)
{
  Matrix m_copie = copie_matrix(m);

  reduced_row_echelon_form(m_copie, NULL);
  displayMatrix(m_copie);
  int r;
  int j;
  for (r = 0, j = 0; r < m->nbrows && j < m->nbcols; ++j) {
    for (;j < m->nbcols; ++j) {
      if (getElt(m_copie, r, j) != 0.0) {
        ++r;
        break;
      }
    }
  }

  deleteMatrix(m_copie);

  return r;
}

static E reduced_row_echelon_form(Matrix A, Matrix B)
{
  E det = 1;

  int i;
  int j;
  for (i = 0, j = 0; i < A->nbrows && j < A->nbcols; ++j) {
    int p = choose_pivot(A, i, j);
    if (p != -1) {
      if (p != i) {
        swap_line(A, i, p);
        if (B != NULL) {
          swap_line(B, i, p);
        }
        det *= -1;
      }

      E c1 = getElt(A, i, j);

      // On met les coefficients au dessus de (i, j) à 0
      for (int k = 0; k < i; ++k) {
        E c2 = -getElt(A, k, j);
        combine_line(A, 1, k, c2 / c1, i);
        if (B != NULL) {
        combine_line(B, 1, k, c2 / c1, i);
        }
      }

      // On met les coefficients en dessous de (i, j) à 0
      for (int k = i + 1; k < A->nbrows; ++k) {
        E c2 = -getElt(A, k, j);
        combine_line(A, c1, k, c2, i);
        if (B != NULL) {
        combine_line(B, c1, k, c2, i);
        }
        det /= c1;
      }

      // On normalise la ligne i
      combine_line(A, 1 / c1, i, 0, 0);
      if (B != NULL) {
        combine_line(B, 1 / c1, i, 0, 0);
      }
      det *= c1;

      ++i;
    }
  }

  return i == j ? det : 0.0;
}

static E valeur_absolue(E e)
{
  return e >= 0 ? e : -e;
}

static int choose_pivot(Matrix m, int row, int col)
{
  int pivot = row;
  E max = valeur_absolue(getElt(m, row, col));
  for(++row; row < m->nbrows; ++row) {
    E value = valeur_absolue(getElt(m, row, col));
    if(value > max) {
      pivot = row;
      max = value;
    }
  }

  pivot = max == 0.0 ? -1 : pivot;
  return pivot;
}

static void swap_line(Matrix m, int l1, int l2)
{
  for (int i = 0; i < m->nbcols; ++i) {
    E tmp = getElt(m, l1, i);
    setElt(m, l1, i, getElt(m, l2, i));
    setElt(m, l2, i, tmp);
  }
}

static void combine_line(Matrix m, E c1, int l1, E c2, int l2)
{
  for (int i = 0; i < m->nbcols; ++i) {
    E value = c1 * getElt(m, l1, i) + c2 * getElt(m, l2, i);
    setElt(m, l1, i, value);
  }
}

static int plus_grande_compo(Matrix A)
{
  int i,ind = 0;
  E max = getElt(A,0,0);
  for(i= 1 ; i < A->nbrows ; i++)
  {
    if (max < getElt(A,i,0))
    {
      ind = 0;
    }
  }

  return ind;
}

static E plus_grande_compo_absolue(Matrix A)
{
  int i;
  E max = valeur_absolue(getElt(A,0,0));
  for(i = 1;i < A->nbrows;i++)
  {
    if (max < valeur_absolue(getElt(A,i,0)))
    {
      max = valeur_absolue(getElt(A,i,0));
    }
  }

  return max;
}

eigenvalue_t eigenvalues(Matrix A,E precision)
{
  if (A->nbrows != A->nbcols) {
    fprintf(stderr, "L'argument 1 n'est pas une matrice carré.\n");
    eigenvalue_t error = {.vecteur_propre = NULL};
    return error;
  }

  if (precision < 0.0) {
    precision = 0.1;
  }

  Matrix m_propre = copie_matrix(A);
  Matrix v_propre = newMatrix(m_propre->nbrows,1);
  E vp=0;
  int i,j;
  j = plus_grande_compo(m_propre);
  for(i=0 ; i < v_propre->nbrows;i++)
  {
    if (i == j) 
    {
      setElt(v_propre, i, 0, 1);
    }
    else 
    {
      setElt(v_propre, i, 0, 0);
    }
  }

  Matrix a_free;
  int ok = 0;
  while(ok == 0)
  {
    a_free = v_propre; 
    v_propre = mult(m_propre,v_propre);
    deleteMatrix(a_free);
  
    if (precision > (plus_grande_compo_absolue(v_propre)-vp))
      ok = 1;

    a_free = v_propre;
    vp = plus_grande_compo_absolue(v_propre);
    v_propre = mult_scal(v_propre,1/vp);
    deleteMatrix(a_free);
  } 
  eigenvalue_t ev = {.valeur_propre = vp, .vecteur_propre = v_propre};
  deleteMatrix(m_propre);
  return ev;
}

least_squares_t least_estimate(Matrix A, char * nom_fichier)
{
  if (A->nbcols != 2) {
    fprintf(stderr, "Le nombre de colonne de la matrice de l'argument 1 doit etre de 2\n");
    least_squares_t error = {.coef_droite = NULL, .residu = NULL};
    return error;
  }

  Matrix new_A = newMatrix(A->nbrows,2);
  Matrix new_B = newMatrix(A->nbrows,1);
  // init matrices
  for(int i = 0; i < A->nbrows; i++) {
    setElt(new_A,i,0,getElt(A,i,0));
    setElt(new_A,i,1,1);
    setElt(new_B,i,0,getElt(A,i,1));
  } 
  Matrix new_A_T = transpose(new_A);
  Matrix A_T_A = mult(new_A_T,new_A);
  Matrix A_T_B = mult(new_A_T,new_B);
  Matrix X = solve(A_T_A,A_T_B);
  Matrix residu = newMatrix(A->nbrows,1);
  E x,y;
  for(int i=0;i<A->nbrows;i++) {
    y = getElt(new_B, i, 0);
    x = (getElt(new_A, i, 0) * getElt(X, 0, 0)) + getElt(X, 1, 0);
    setElt(residu, i, 0, (y - x));
  }

  if (nom_fichier != NULL) {
    FILE * file = fopen(nom_fichier,"w");

    fprintf(file, "set xlabel \"Axe des x\"\n");
    fprintf(file, "set ylabel \"Axe des y\"\n");
    fprintf(file, "f(x)=x*%g+%g\n",getElt(X,0,0),getElt(X,1,0));
    fprintf(file, "set terminal png size 640,480\n"); 
    fprintf(file, "set output \"%s.png\"\n", nom_fichier);

    fprintf(file, "plot f(x)\n");
    fprintf(file, "plot \"-\" with points\n");
    for (int i = 0; i < A->nbrows; ++i) {
      fprintf(file, "%g %g\n", getElt(A, i, 0), getElt(A, i, 1));
    }
    fprintf(file, "end\n");

    fclose(file);
  }
  
  least_squares_t ls = {.residu = residu, .coef_droite = X};

  deleteMatrix(new_A);
  deleteMatrix(new_A_T);
  deleteMatrix(new_B);
  deleteMatrix(A_T_A);
  deleteMatrix(A_T_B);
  
  return ls;
}
