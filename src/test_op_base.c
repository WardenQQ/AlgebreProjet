#include "matrix.h"
#include "operation.h"

int main()
{
  Matrix a = newMatrix(3,3);
  Matrix m;
  a = aleatoire(a,0,100);
  
  displayMatrix(a);
  /*
  Matrix m = addition(a,a);

  displayMatrix(a);

  displayMatrix(m);
  
  m = sub(a,a);

  displayMatrix(a);

  displayMatrix(m);
  
  m = mult(a,a);

  
  displayMatrix(a);

  displayMatrix(m);
  
  m = mult_scal(a,3);

  
  displayMatrix(a);

  displayMatrix(m);
  
  m = expo(a,5);


  displayMatrix(a);

  displayMatrix(m);
  */
  m = transpose(a);

  displayMatrix(a);

  displayMatrix(m);
  

  deleteMatrix(a);
  deleteMatrix(m);
  return 0;
}





