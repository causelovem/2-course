#ifndef Rational_number_h
#define Rational_number_h

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Big_int.hpp"

class Rational_number     /*just rational number class, with big int numepator and denominator*/
{
    private:
        Big_int numerator;
        Big_int denominator;

    public:
        Rational_number operator = (const Rational_number &arg);
        Rational_number operator = (const long int arg);

        int operator == (const Rational_number &arg)const;
        int operator == (const long int arg)const;
        friend int operator == (const long int arg1, const Rational_number &arg2);

        int operator != (const Rational_number &arg)const;
        int operator != (const long int arg)const;
        friend int operator != (const long int arg1, const Rational_number &arg2);

        int operator > (const Rational_number &arg)const;
        int operator > (const long int arg)const;
        friend int operator > (const long int arg1, const Rational_number &arg2);

        int operator < (const Rational_number &arg)const;
        int operator < (const long int arg)const;
        friend int operator < (const long int arg1, const Rational_number &arg2);

        int operator >= (const Rational_number &arg)const;
        int operator >= (const long int arg)const;
        friend int operator >= (const long int arg1, const Rational_number &arg2);

        int operator <= (const Rational_number &arg)const;
        int operator <= (const long int arg)const;
        friend int operator <= (const long int arg1, const Rational_number &arg2);

        Rational_number operator + (const Rational_number &arg)const;
        Rational_number operator + (const long int arg)const;
        friend Rational_number operator + (const long int arg1, const Rational_number &arg2);

        Rational_number operator - (const Rational_number &arg)const;
        Rational_number operator - (const long int arg)const;
        friend Rational_number operator - (const long int arg1, const Rational_number &arg2);

        Rational_number operator * (const Rational_number &arg)const;
        Rational_number operator * (const long int arg)const;
        friend Rational_number operator * (const long int arg1, const Rational_number &arg2);

        Rational_number operator / (const Rational_number &arg)const;
        Rational_number operator / (const long int arg)const;
        friend Rational_number operator / (const long int arg1, const Rational_number &arg2);

        Rational_number operator ++ ();
        Rational_number operator -- ();

        void show(FILE *f)const;

        void make_canonical();
        void round();
        void floor();

        short rattoshort ();
        int rattoint ();
        long int rattolong ();

        Rational_number();
        Rational_number(const Big_int &arg);
        Rational_number(const Rational_number &arg);
        Rational_number(const long int arg);
        Rational_number(const char *arg1, const char *arg2);
        Rational_number(const char *arg);
};

#endif