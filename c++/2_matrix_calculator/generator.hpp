#ifndef generator_h
#define generator_h

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Exception.hpp"
#include "class_Big_int.hpp"
#include "class_Rational_number.hpp"
#include "class_Vector.hpp"
#include "class_Matrix.hpp"

Rational_number gen_rn(long int left, long int right);

Vector gen_vec(long int left, long int right, long int vs);

Matrix gen_mat(long int left, long int right, long int rs, long int cs);

#endif