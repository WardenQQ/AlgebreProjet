#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"

/**
 *
 * \brief Fonction de création de Matrices
 *
 * \param nb_rows Nombre de lignes
 * \param nb_columns Nombre de colonnes
 * \return Retourne une matrice à nb_rows lignes et nb_columns colonnes.
 * Ou NULL si les nb_rows < 1 || nb_columns < 1.
 */
Matrix newMatrix(int nb_rows, int nb_columns)
{
  Matrix m;
  if (nb_rows < 1 || nb_columns < 1) {
    fprintf(stderr, "La matrice doit etre de taille minimum 1x1.\n");
    return NULL;
  }

  if ((m = malloc(sizeof(struct matrix))) == NULL) {
    perror("Erreur de malloc ");
    return NULL;
  }

  if ((m->mat = malloc(sizeof(E) * (nb_rows * nb_columns))) == NULL) {
    perror("Erreur de malloc ");
    return NULL;
  }

  m->nbrows = nb_rows;
  m->nbcols = nb_columns;

  return m;
}

/**
 * \brief Fonction d'accession à un élément i,j de la Matrice
 * 
 * \param m Matrice
 * \param rows Ligne de la Matrice
 * \param columns Colonne de la Matrice
 * \return Retourne un élément de la Matrice à la ligne rows 
 * et à la colonne columns
 */
E getElt(Matrix m,int rows,int columns)
{
  return m->mat[columns + (rows * (m->nbcols))];
}

/**
 * \brief Fonction de modification d'un élément en i,j de la Matrice
 *
 * \param m Matrice
 * \param row Ligne de la Matrice 
 * \param column Colonne de la Matrice
 *
 */

void setElt(Matrix m,int row,int column,E val)
{
  m->mat[column + (row * m->nbcols)] = val;
}

/**
 * \brief Fonction de suppression d'une Matrice
 *
 * \param m Matrice
 * 
 */

void deleteMatrix(Matrix m)
{
  free(m->mat);
  free(m);
}

void displayMatrix(Matrix m)
{
  for (int i = 0; i < m->nbrows; ++i) {
    printf("    [ ");
    for (int j = 0; j < m->nbcols; ++j) {
      printf("%- 8.2f ", getElt(m, i, j));
    }
    printf("]\n");
  }
}

Matrix identite(int nb_rows,int nb_columns)
{
  Matrix m = newMatrix(nb_rows,nb_columns);

  for (int i = 0; i < m->nbrows; ++i) {
    for (int j = 0; j < m->nbcols; ++j) {
      if (i == j) {
	setElt(m, i, j, 1.0);
      } else {
	setElt(m, i, j, 0.0);
      }
    }
  }

  return m;
}

Matrix aleatoire(int nb_rows, int nb_cols, E min, E max)
{
  int i,j;
  Matrix m = newMatrix(nb_rows,nb_cols);
  srand48(time(NULL));
  for(i=0;i<m->nbrows;i++)
    for(j=0;j<m->nbcols;j++)
      setElt(m,i,j,(drand48()*(max-min))+min);

  return m;
}
