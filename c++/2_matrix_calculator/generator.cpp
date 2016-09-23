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
#include "generator.hpp"

Rational_number gen_rn(long int left, long int right)
{

	return Rational_number(rand() % (right - left + 1) + left);
}

Vector gen_vec(long int left, long int right, long int vs)
{
	Vector res("0",vs);

	for (int i = 1; i < vs + 1; i++)
		res.push(gen_rn(left, right), i);

	return res;
}

Matrix gen_mat(long int left, long int right, long int rs, long int cs)
{
	Matrix res("0", rs, cs);

	for (int i = 1; i < rs + 1; i++)
		for (int j = 1; j < cs + 1; j++)
			res.push(gen_rn(left, right), i, j);

	return res;
}