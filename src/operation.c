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
        mult = mult + (getElt(m1,i,k) * getElt(m2,k,j));		}	
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

