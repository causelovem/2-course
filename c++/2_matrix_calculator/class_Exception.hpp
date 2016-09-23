#ifndef Exeprion_h
#define Exeprion_h

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Big_int.hpp"
#include "class_Rational_number.hpp"

class Exception
{
    private:
        char *err_message;

    public:
        Exception();
        Exception(const char *arg);
        Exception(const Exception &arg);

        void show(FILE *f)const;

        ~Exception();
};

class Exception_vector_border: protected Exception
{
    private:
        long int size, required_size;

    public:
        Exception_vector_border(const long int &arg1, const long int &arg2)
        {
            size = arg1;
            required_size = arg2;
            
            return;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Out of border.\nRequired corder (%li), vector size (%li).", required_size, size);

            return;
        };
};

class Exception_bigtoshort: protected Exception
{
    private:
        Big_int num;

    public:
        Exception_bigtoshort(const Big_int &arg)
        {
            num = arg;

            return;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Impossible to convert Big To Short ");
            num.show(f);

            return;
        };  
};

class Exception_bigtoint: protected Exception
{
    private:
        Big_int num;

    public:
        Exception_bigtoint(const Big_int &arg)
        {
            num = arg;

            return;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Impossible to convert Big To Int ");
            num.show(f);

            return;
        };  
};

class Exception_bigtolong: protected Exception
{
    private:
        Big_int num;

    public:
        Exception_bigtolong(const Big_int &arg)
        {
            num = arg;

            return;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Impossible to convert Big To Long ");
            num.show(f);

            return;
        };  
};

class Exception_rattoshort: protected Exception
{
    private:
        Rational_number num;

    public:
        Exception_rattoshort(const Rational_number &arg)
        {
            num = arg;

            return;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Impossible to convert Rational To Short ");
            num.show(f);

            return;
        };  
};

class Exception_rattoint: protected Exception
{
    private:
        Rational_number num;

    public:
        Exception_rattoint(const Rational_number &arg)
        {
            num = arg;

            return;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Impossible to convert Rational To Int ");
            num.show(f);

            return;
        };  
};

class Exception_rattolong: protected Exception
{
    private:
        Rational_number num;

    public:
        Exception_rattolong(const Rational_number &arg)
        {
            num = arg;

            return;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Impossible to convert Rational To Long ");
            num.show(f);

            return;
        };  
};

class Exception_file_name: protected Exception
{
    private:
        char *filename;

    public:
        Exception_file_name(const char *arg)
        {
            filename = (char *) malloc ((strlen(arg) + 1) * sizeof(char));

            for (int i = 0; i < strlen(arg); i++)
                filename[i] = arg[i];

            filename[strlen(arg)] = '\0';

            return;
        };

        Exception_file_name(const Exception_file_name &arg)
        {
            filename = (char *) malloc ((strlen(arg.filename) + 1) * sizeof(char));
            for (int i = 0; i < strlen(arg.filename); i++)
                filename[i] = arg.filename[i];

            filename[strlen(arg.filename)] = '\0';

            return;
        };

        ~Exception_file_name()
        {
            if (filename != NULL)
                free(filename);

            filename = NULL;

            return;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Can not find file '%s'.\n", filename);

            return;
        }; 
};

class Exception_matrix_border: protected Exception
{

    private:
        long int col_count, row_count;
        long int col_size, row_size; 

    public:
        Exception_matrix_border(const long int rc, const long int cc, const long int rs, const long int cs)
        {
            row_count = rc;
            col_count = cc;
            row_size = rs;
            col_size = cs;

            return;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Out of border.\nRequired corder (%li, %li), matrix size (%li, %li).", row_count, col_count, row_size, col_size);

            return;
        }
};

class Exception_matrix_sizes: protected Exception
{
    private:
        long int row1, col1, row2, col2;

    public:
        Exception_matrix_sizes(const long int r1, const long int c1, const long int r2, const long int c2)
        {
            row1 = r1;
            col1 = c1;
            row2 = r2;
            col2 = c2;
        };

        void show(FILE *f)const
        {
            fprintf(f, ">Mismatched sizes of matrix for summation: (%li, %li) and (%li, %li)", row1, col1, row2, col2);

            return;
        }
};

#endif