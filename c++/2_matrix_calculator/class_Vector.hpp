#ifndef Vector_h
#define Vector_h

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Exception.hpp"
#include "class_Big_int.hpp"
#include "class_Rational_number.hpp"

struct Dictionary_vector
{
    long int position;
    Rational_number value;
};

class Vector
{
    private:
        Dictionary_vector *vec;
        long int vec_len;
        long int vector_len;

    public:
        Vector operator = (const Vector &arg);

        Vector operator + (const Vector &arg)const;
        Vector operator - (const Vector &arg)const;

        Vector operator - ();

        Vector operator * (const Rational_number &arg)const;
        friend Vector operator * (const Rational_number &arg1, const Vector &arg2);

        Vector operator / (const Rational_number &arg)const;

        Rational_number operator [] (const long int arg)const;

        void show(FILE *f)const;

        long int get_vector_len()const;
        long int get_vec_len()const;

        void push(const Rational_number &number, const long int arg);

        void write(const char *file);

        Vector();
        Vector(const Rational_number &nuber, const int size);
        Vector(const Vector &arg);
        Vector(const char *file);
        ~Vector();
};

#endif