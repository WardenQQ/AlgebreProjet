#ifndef __OPERATION_H_
#define __OPERATION_H_
#include "matrix.h"


Matrix addition(Matrix m1,Matrix m2);

Matrix mult(Matrix m1,Matrix m2);

Matrix sub(Matrix m1,Matrix m2);

Matrix transpose(Matrix m);

Matrix mult_scal(Matrix m,E s);

#endif
