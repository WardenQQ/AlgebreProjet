#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"


Matrix newMatrix(int nb_rows,int nb_columns)
{
	Matrix m;
	if ((m = malloc(sizeof(struct matrix))) == NULL)
	{
		perror("Erreur de maloc :");
	}
	
	if ((m->mat = (E *)malloc(sizeof(E) * (nb_rows * nb_columns))) == NULL)
	{
		perror("Erreur de maloc :");
	}
	m->nbrows = nb_rows;
	m->nbcols = nb_columns;

	return m;
}

E getElt(Matrix m,int rows,int columns)
{
	return (m->mat[columns+(rows*(m->nbcols))]);//num de la colonne + (nombre de colonne * la ligne)
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
	int i = 0;
	int j = 0;
	while (i < m->nbrows)
	{
		while (j< m->nbcols)
		{
			if (j == i)
			{
				setElt(m,i,j,1);
			}
			j++;	
		}
		i ++;
		j = 0;	
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






