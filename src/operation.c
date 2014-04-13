#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "limits.h"
#include "operation.h"

static E valeur_absolue(E e);
static int choixPivot(Matrix m, int i);
static int choose_pivot(Matrix m, int row, int col);
static void swap_line(Matrix m, int l1, int l2);
static void combine_line(Matrix m, E c1, int l1, E c2, int l2);
static E reduced_row_echelon_form(Matrix A, Matrix B);

static int choixPivot(Matrix m, int i)
{
  return i;
}

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

int determinant_bis(Matrix m, E *det)
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

Matrix invert_bis(Matrix m)
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

Matrix solve_bis(Matrix A, Matrix B)
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

int rank_bis(Matrix m)
{
  Matrix m_copie = copie_matrix(m);

  reduced_row_echelon_form(m_copie, NULL);

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
      E c2;

      // On met les coefficients au dessus de (i, j) à 0
      for (int k = 0; k < i; ++k) {
        E c2 = -getElt(A, k, j);
        combine_line(A, 1, k, c2 / c1, i);
        assert(getElt(A, k, j) == 0.0);
        if (B != NULL) {
        combine_line(B, 1, k, c2 / c1, i);
        }
      }

      // On met les coefficients en dessous de (i, j) à 0
      for (int k = i + 1; k < A->nbrows; ++k) {
        E c2 = -getElt(A, k, j);
        combine_line(A, c1, k, c2, i);
        assert(getElt(A, k, j) == 0.0);
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

void echangeLigne_det(Matrix m,int i,int j)
{
  int k;
  E tmp;
  for(k = 0;k < m->nbrows; k++)
  {
    tmp = getElt(m,i,k);
    setElt(m,i,k,getElt(m,j,k));
    setElt(m,j,k,tmp);
  }
}

void echangeLigne(Matrix A,Matrix B,int i,int j)
{
  int k;
  E tmp;
  for(k = 0;k < A->nbrows;k++)
  {
    tmp = getElt(A,i,k);
    setElt(A,i,k,getElt(A,j,k));
    setElt(A,j,k,tmp);
  }
  tmp = getElt(B,i,0);
  setElt(B,i,0,getElt(B,j,0));
  setElt(B,j,0,tmp);
}

void addmultiple_det(Matrix m , int i , int j ,E c)
{
  int k;
  E e;
  for(k = 0;k < m->nbrows;k++)
  {
    e = getElt(m,i,k) + c * getElt(m,j,k);
    setElt(m,i,k,e);
  }

}

E triangulaire_det(Matrix m)
{
  E c=1;
  int i,j;
  for(i = 0;i < m->nbrows-1 ; i++)
  {
    j = choixPivot(m,i);
    if (j!=i)
    {
      echangeLigne_det(m,i,j);
      c = -c;
    }
    for(j = i+1;j < m->nbrows ; j++)
    {
      addmultiple_det(m,j,i,-(getElt(m,j,i)/getElt(m,i,i)));
    }  
  }
  return c;
}


E determinant(Matrix m)
{
  if ((m->nbrows == 1) && (m->nbcols == 1))
  {
    return (getElt(m,0,0));
  }
  else if (m->nbrows != m->nbcols)
  {
    fprintf(stderr,"Matrice non carré\n");
    exit(1);
  }
  
  Matrix tmp = copie_matrix(m);
  E c = triangulaire_det(tmp);
  for(int i= 0; i < tmp->nbrows; i++) {
    c = c * getElt(tmp,i,i);
  }
  deleteMatrix(tmp);

  return c;
}

Matrix invert(Matrix m)
{
  if (m->nbrows != m->nbcols)
  {
    printf("Matrice non carré colonnes != lignes \n");
    return NULL;
  }
  if (determinant(m) == 0)
  {
    printf("Matrice non inversible determinant = 0 \n");
    return NULL;
  }
  Matrix m_inv; 
  Matrix co_m = comatrice(m);
  Matrix co_t = transpose(co_m); 
  m_inv = mult_scal(co_t,(1/determinant(m)));
 
  deleteMatrix(co_m);
  deleteMatrix(co_t); 
  return m_inv;
}

Matrix solve(Matrix A,Matrix B)
{
  Matrix X = newMatrix(A->nbrows,1);
  Matrix new_A = copie_matrix(A);
  Matrix new_B = copie_matrix(B);
  triangulaire(new_A,new_B);
  remontee(new_A,new_B,X);
  deleteMatrix(new_A);
  deleteMatrix(new_B);  
  return X;
}

int rank(Matrix A)
{
  int i,j;
  int rank = 0;
  Matrix tmp = copie_matrix(A);
  triangulaire_det(tmp);
  for(i=0;i < tmp->nbrows;i++)
  {
    for(j=0;j < tmp->nbcols;j++)
    {
      if (getElt(tmp,i,j) != 0)
      { 
	rank++;
	break;
      }
    }
  }
  deleteMatrix(tmp);

  return rank;
}

Matrix extraction(Matrix m,int i ,int j )
{
  int k,l;
  k = 0; l = 0;
  Matrix m_ex = newMatrix(m->nbrows-1,m->nbcols-1);
  
  
  for(k=0;k<i;k++)
      for(l=0;l<j;l++)
	setElt(m_ex,k,l,getElt(m,k,l));
  
  for(k=0;k<i;k++)
      for(l=j+1;l<m->nbcols;l++)
	setElt(m_ex,k,l-1,getElt(m,k,l));

   
  for(k=i+1;k<m->nbrows;k++)
      for(l=0;l<j;l++)
	setElt(m_ex,k-1,l,getElt(m,k,l));

  for(k=i+1;k<m->nbrows;k++)
      for(l=j+1;l<m->nbrows;l++)
	setElt(m_ex,k-1,l-1,getElt(m,k,l));
 

   
  return m_ex;
}

int puisscom(int i ,int j)
{
    int puiss = i + j;
    int rsl = -1;
    while(puiss > 1)
    {
	rsl = rsl * -1;
	puiss --;
    }
    return rsl;
}

Matrix comatrice(Matrix m)
{
  Matrix co_m = newMatrix(m->nbrows,m->nbcols);
  int i,j;
  for(i =0;i<m->nbrows;i++)
  {
    for (j=0;j<m->nbcols;j++)
    {
       Matrix tmp = extraction(m,i,j);
       setElt(co_m,i,j,puisscom(i+1,j+1)*determinant(tmp));
       deleteMatrix(tmp);
    }
  }
  
  return co_m;
}

void remontee(Matrix A,Matrix B,Matrix X)
{
  int i,j;
  for(i=A->nbrows-1;i>=0;i--)
  {
    setElt(X,i,0,getElt(B,i,0));
    for(j=i+1;j<A->nbrows;j++)
    {
      setElt(X,i,0,(getElt(X,i,0)-(getElt(A,i,j)*getElt(X,j,0))));
    }
    setElt(X,i,0,getElt(X,i,0)/getElt(A,i,i));
  }
}

void addmultiple(Matrix A,Matrix B,int i,int j,E c)
{
  int k= 0;
  for(k=0;k<A->nbrows;k++)
  {
    setElt(A,i,k,(getElt(A,i,k) + (c * getElt(A,j,k))));
  }
  setElt(B,i,0,(getElt(B,i,0) + c * getElt(B,j,0)));
}

void triangulaire(Matrix A,Matrix B)
{
  int i,j;
  i = 0;
  j = 0;
  E c = 0;
  for(i=0;i<A->nbrows-1;i++)
  {
    j = choixPivot(A,i);
    echangeLigne(A,B,i,j);
    for(j=i+1;j<A->nbrows;j++)
    {
      c = -(getElt(A,j,i)/getElt(A,i,i)); 
      addmultiple(A,B,j,i,c);
    }
  } 
}




int plus_grande_compo(Matrix A)
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

E plus_grande_compo_absolue(Matrix A)
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

E valeur_propre(Matrix A,E precision)
{
  Matrix m_propre = copie_matrix(A);
  Matrix v_propre = newMatrix(m_propre->nbrows,1);
  E vp=0;
  int i,j;
  j = plus_grande_compo(m_propre);
  for(i=0 ; i < v_propre->nbrows;i++)
  {
    if (i == j) 
    {
      setElt(v_propre,i,0,1);
    }
    else 
    {
      setElt(v_propre,i,0,0);
    }
  }
  /*   
  v_propre = mult(m_propre,v_propre);
  Matrix a_free = v_propre;
  vp = plus_grande_compo_absolue(v_propre);
  v_propre = mult_scal(v_propre,1/vp);
  deleteMatrix(a_free);*/
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
  deleteMatrix(v_propre);
  deleteMatrix(m_propre);
  return vp;
}

Matrix least_estimate(Matrix A,char * nom_fichier)
{
  if (nom_fichier == NULL)
  {
      return NULL;
  }
  FILE * file = fopen(nom_fichier,"w");
  int i;

  Matrix new_A = newMatrix(A->nbrows,2);
  Matrix new_B = newMatrix(A->nbrows,1);
  // init matrices
  for(i = 0;i<A->nbrows;i++)
  {
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
  for(i=0;i<A->nbrows;i++)
  {
    y = getElt(new_B,i,0);
    x = (getElt(new_A,i,0) * getElt(X,0,0)) + getElt(X,1,0);
    setElt(residu,i,0,(y-x));
  }

  
  fprintf(file, "set xlabel \"Axe des x\"\n");
  fprintf(file, "set ylabel \"Axe des y\"\n");
  fprintf(file, "f(x)=x*%f+%f\n",getElt(X,0,0),getElt(X,1,0));
  //fprintf(file, "set xrange[-20:20]\n");
  //fprintf(file, "set yrange[-20:20]\n");
  fprintf(file, "set terminal png size 640,480 enhanced font \"Helvetica,20\"\n"); 
  fprintf(file, "set output \"graph.png\"\n");
  fprintf(file, "plot f(x)\n");

  deleteMatrix(new_A);
  deleteMatrix(new_A_T);
  deleteMatrix(new_B);
  deleteMatrix(A_T_A);
  deleteMatrix(A_T_B);
  fclose(file);
  return X;
}



static void speedtest_addition(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);
    Matrix B = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = addition(A, B);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(B);
    deleteMatrix(C);
  }

  fclose(file);
}


static void speedtest_sub(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);
    Matrix B = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = sub(A, B);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(B);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_mult(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);
    Matrix B = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = mult(A, B);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(B);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_mult_scal(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = mult_scal(A,(E)i);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_expo(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = expo(A,i);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_transpose(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = transpose(A);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_determinant(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    E det = determinant(A);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
  }

  fclose(file);
}

static void speedtest_invert(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = invert(A);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_solve(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX);
    Matrix B = aleatoire(i, 0, DBL_MIN, DBL_MAX);

    gettimeofday(&start, NULL);
    Matrix C = solve(A, B);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
    deleteMatrix(B);
    deleteMatrix(C);
  }

  fclose(file);
}

static void speedtest_rank(int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./plot.dat", "w");
  struct timeval start;
  struct timeval end;

  unsigned long long t = 0;
  for (int i = taille_min; i <= taille_max && t < nb_sec * 1000000; i += pas) {
    Matrix A = aleatoire(i, i, DBL_MIN, DBL_MAX); 

    gettimeofday(&start, NULL);
    int rang = rank(A);
    gettimeofday(&end, NULL);

    t = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    fprintf(file, "%d %llu\n", i, t);

    deleteMatrix(A);
  }

  fclose(file);
}


void speedtest(char *commande, int taille_min, int taille_max, int pas, int nb_sec)
{
  FILE * file = fopen("./graph", "w");

  fprintf(file, "set xlabel \"Taille des matrices\"\n");
  fprintf(file, "set ylabel \"Temps en microsecondes\"\n");
  fprintf(file, "set terminal png size 640,480 enhanced font \"Helvetica,20\"\n");
  fprintf(file, "set output \"graph.png\"\n");
  fprintf(file, "plot \"plot.dat\" with lines\n");

  if (strncmp(commande, "addition", STRING_MAX) == 0) {
    speedtest_addition(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "sub", STRING_MAX) == 0) {
    speedtest_sub(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "mult", STRING_MAX) == 0) {
    speedtest_mult(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "mult_scal", STRING_MAX) == 0) {
    speedtest_mult_scal(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "expo", STRING_MAX) == 0) {
    speedtest_expo(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "transpose", STRING_MAX) == 0) {
    speedtest_transpose(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "determinant", STRING_MAX) == 0) {
    speedtest_determinant(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "invert", STRING_MAX) == 0) {
    speedtest_invert(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "solve", STRING_MAX) == 0) {
    speedtest_solve(taille_min, taille_max, pas, nb_sec);
  } else if (strncmp(commande, "rank", STRING_MAX) == 0) { 
    speedtest_rank(taille_min, taille_max, pas, nb_sec);
  }

  fclose(file);
}
