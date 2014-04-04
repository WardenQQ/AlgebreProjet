#include "operation.h"

Matrix addition(Matrix m1,Matrix m2)
{
  if ((m1->nbcols != m2->nbcols) || (m1->nbrows != m2->nbrows))	
  {
    fprintf(stderr,"Addition impossible");
    exit(1);
  }
  Matrix new_a = newMatrix(m1->nbrows,m1->nbcols);
  int i,j;
	
  for(i = 0;i < m1->nbrows;i++)
    for(j = 0;j < m1->nbcols;j++)
      setElt(new_a,i,j,getElt(m1,i,j)+getElt(m2,i,j));

  return new_a;
}

Matrix mult(Matrix m1,Matrix m2)
{
  if (m1->nbcols != m2->nbrows)
  {
    fprintf(stderr,"Multiplication impossible");
    exit(1);
  }
	
  Matrix new_a = newMatrix(m1->nbrows,m2->nbcols);
  int i,j,k;
  E mult = 0;
  for(i = 0;i < m1->nbrows;i++)
  {
    for(j = 0;j < m1->nbcols;j++)
    {
      for(k = 0;k < m1->nbcols ;k++)
      {
        mult = mult + (getElt(m1,i,k) * getElt(m2,k,j));
      }	
    setElt(new_a,i,j,mult);
    mult = 0;
    }
  }

  return new_a;
}

Matrix sub(Matrix m1,Matrix m2)
{
  if ((m1->nbrows != m2->nbrows) || (m1->nbcols != m2->nbcols))
  {
    fprintf(stderr,"Mauvaise dimension");
    exit(1);
  }
  Matrix m_sub = newMatrix(m1->nbrows,m1->nbcols);
  int i,j;
  for(i = 0;i<m1->nbrows;i++)
    {
      for(j = 0;j<m1->nbcols;j++)
        setElt(m_sub,i,j,getElt(m1,i,j)-getElt(m2,i,j));
    }

  return m_sub;
}

Matrix transpose(Matrix m)
{
  int i,j;
  Matrix m_t = newMatrix(m->nbcols,m->nbrows);
  for(i = 0;i < m->nbrows;i++)
  {
    for(j = 0;j < m->nbcols;j++)
    {
      setElt(m_t,j,i,getElt(m,i,j));
    }
  }
 
  return m_t;
}

Matrix mult_scal(Matrix m ,E s)
{	
  int i,j;
  E e;
  Matrix m_s = newMatrix(m->nbrows,m->nbcols);  
  for(i = 0;i < m->nbrows;i++)
  {
    for(j = 0;j < m->nbcols;j++)
    {
      e = getElt(m,i,j)*s;
      setElt(m_s,i,j,e);
    }
  }
  return m_s;
}

Matrix expo(Matrix m,int exposant)
{
  Matrix new_m = newMatrix(m->nbrows,m->nbcols);
  new_m = copie_matrix(m);
  for(;exposant>1;exposant--)
    new_m = mult(new_m,new_m);

  return new_m; 
}

E valeur_absolue(E e)
{
	if (e < 0)
	{
		return (-e);
	}
	else return e;
}

int choixPivot(Matrix m,int i)
{
  int p = i;
  int j;
  E v = valeur_absolue(getElt(m,i,i));
  for(j = i + 1; j < m->nbcols;j++)
  {
    if(valeur_absolue(getElt(m,j,i)) > v)
    {
      p = j;
      v = valeur_absolue(getElt(m,j,i));
    }
  }
  return p;
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
  E c;
  int i;
  if ((m->nbrows == 1) && (m->nbcols == 1))
  {
    return (getElt(m,0,0));
  }
  Matrix tmp = newMatrix(m->nbrows,m->nbcols);
  tmp = copie_matrix(m);
  c = triangulaire_det(tmp);
  for(i= 0;i<tmp->nbrows;i++)
  {
    c = c * getElt(tmp,i,i);
  }
  deleteMatrix(tmp);
  return c;
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
 

  displayMatrix(m_ex); 
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
    printf("RSL : %d\n",rsl);
    return rsl;
}

Matrix comatrice(Matrix m)
{
  Matrix co_m = newMatrix(m->nbrows,m->nbcols);
  int i,j;
  E co;
  for(i =0;i<m->nbrows;i++)
  {
    for (j=0;j<m->nbcols;j++)
    {
       Matrix tmp = extraction(m,i,j);
       setElt(co_m,i,j,puisscom(i+1,j+1)*determinant(tmp));
       free(tmp);
    }
  }
  
  return co_m;
}

Matrix invert(Matrix m)
{
  if (m->nbrows != m->nbcols)
  {
    printf("Matrice non carré colonnes != lignes");
    exit(1);
  }
  if (determinant(m) == 0)
  {
    printf("Matrice non inversible determinant = 0");
    exit(1);
  }
  Matrix m_inv = newMatrix(m->nbrows,m->nbcols); 
  m_inv = copie_matrix(m);
  printf("1 / DET %f\n",1/determinant(m_inv)); 
  m_inv = mult_scal(transpose(comatrice(m_inv)),(1/determinant(m_inv)));

  return m_inv;
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


Matrix PivotDeGauss(Matrix A,Matrix B,Matrix X)
{
  Matrix new_A = newMatrix(A->nbrows,A->nbcols);
  Matrix new_B = newMatrix(B->nbrows,B->nbcols);
  new_A = copie_matrix(A);
  new_B = copie_matrix(B);
  triangulaire(new_A,new_B);
  displayMatrix(new_A);
  displayMatrix(new_B);
  remontee(new_A,new_B,X);
  
  return X;
}








