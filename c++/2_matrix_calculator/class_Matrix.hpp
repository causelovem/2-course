#ifndef Matrix_h
#define Matrix_h

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Exception.hpp"
#include "class_Big_int.hpp"
#include "class_Rational_number.hpp"
#include "class_Vector.hpp"

struct Dictionary_matrix
{
    Rational_number value;
    long int row_position, col_position;
};

struct Coords
{
    long int col_cord, row_cord;

    Coords()
    {
        col_cord = 0;
        row_cord = 0;
    };

    Coords (long int arg1, long int arg2)
    {
        row_cord = arg1;
        col_cord = arg2;
    };
};

struct Row_position
{
    long int row_pos;

    Row_position()
    {
        row_pos = 0;
    };

    Row_position (long int arg)
    {
        row_pos = arg;
    };
};

struct Col_position
{
    long int col_pos;

    Col_position()
    {
        col_pos = 0;
    };

    Col_position (long int arg)
    {
        col_pos = arg;
    };
};

class Matrix
{
    private:
        Dictionary_matrix *mat;
        long int mat_len;
        long int row_size, col_size;
        
    public:

        Matrix operator = (const Matrix &arg);

        Matrix operator + (const Matrix &arg)const;
        Matrix operator - (const Matrix &arg)const;
        Matrix operator - ();

        Matrix operator * (const Matrix &arg)const;
        Matrix operator * (const Rational_number &arg)const;
        friend Matrix operator * (const Rational_number &arg1, const Matrix &arg2);

        friend Matrix operator * (const Vector &arg1, const Matrix &arg2);
        friend Matrix operator * (const Matrix &arg1, const Vector &arg2);

        Matrix operator / (const Rational_number &arg)const;

        Rational_number operator [] (const Coords &arg)const;
        Matrix operator [] (const Row_position &arg)const;
        Matrix operator [] (const Col_position &arg)const;

        Matrix operator ~ ()const;

        void show(FILE *f)const;

        void push(const Rational_number &number, const long int row, const long int col);

        void write(const char *file);
        
        Matrix();
        Matrix(const Rational_number &number, const long int row, const long int col);
        Matrix(const Matrix &arg);
        Matrix(const Vector &arg);
        Matrix(const char *file);
        ~Matrix();    
};

#endif
