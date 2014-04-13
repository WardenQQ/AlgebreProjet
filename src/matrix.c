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

E getElt(Matrix m,int rows,int columns)
{
  return m->mat[columns + (rows * (m->nbcols))];
}

void setElt(Matrix m,int row,int column,E val)
{
  m->mat[column + (row * m->nbcols)] = val;
}

void deleteMatrix(Matrix m)
{
	free(m->mat);
	free(m);
}

// TODO: Affichage de la matrice
void displayMatrix(Matrix m)
{
	int i,j;
	i = 0;
	j = 0;
	while(i < m->nbrows)
	{
		while(j < m->nbcols)
		{
			printf("%3.2f ",getElt(m,i,j));
			j++; 	
		}
		printf("\n");
		j = 0;
		i ++;
	}
	printf("\n");
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
