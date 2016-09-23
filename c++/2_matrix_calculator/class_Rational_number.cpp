#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Exception.hpp"
#include "class_Big_int.hpp"
#include "class_Rational_number.hpp"

using namespace std;

Rational_number Rational_number::operator = (const Rational_number &arg)
{
    this->numerator = arg.numerator;
    this->denominator = arg.denominator;

    return (*this);
}

Rational_number Rational_number::operator = (const long int arg)
{
    Rational_number arg1(arg);

    return (*this = arg1);
}

int Rational_number::operator == (const Rational_number &arg)const
{
    Rational_number arg1(*this), arg2(arg);

    arg1.make_canonical();
    arg2.make_canonical();

    if ((arg1.numerator == arg2.numerator) && (arg1.denominator == arg2.denominator))
        return 1;

    return 0;
}

int Rational_number::operator == (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this == arg1);
}

int operator == (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg == arg2);
}

int Rational_number::operator != (const Rational_number &arg)const
{
    Rational_number arg1(*this), arg2(arg);

    arg1.make_canonical();
    arg2.make_canonical();

    if ((arg1.numerator != arg2.numerator) || (arg1.denominator != arg2.denominator))
        return 1;

    return 0;
}

int Rational_number::operator != (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this != arg1);
}

int operator != (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg != arg2);
}

int Rational_number::operator > (const Rational_number &arg)const
{
    Rational_number arg1(*this), arg2(arg), tmp;

    if ((arg1.numerator > 0) && (arg2.numerator < 0))
        return 1;
    else
    if ((arg1.numerator < 0) && (arg2.numerator > 0))
        return 0;

    arg1.make_canonical();
    arg2.make_canonical();

    tmp = arg1 - arg2;

    if (tmp.numerator > 0)
        return 1;
    else
        return 0;
}

int Rational_number::operator > (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this > arg1);
}

int operator > (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg > arg2);
}

int Rational_number::operator < (const Rational_number &arg)const
{
    Rational_number arg1(*this), arg2(arg), tmp;

    if ((arg1.numerator < 0) && (arg2.numerator > 0))
        return 1;
    else
    if ((arg1.numerator > 0) && (arg2.numerator < 0))
        return 0;

    arg1.make_canonical();
    arg2.make_canonical();

    tmp = arg1 - arg2;

    if (tmp.numerator < 0)
        return 1;
    else
        return 0;
}

int Rational_number::operator < (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this < arg1);
}

int operator < (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg < arg2);
}

int Rational_number::operator >= (const Rational_number &arg)const
{
    Rational_number arg1(*this), arg2(arg), tmp;

    if (arg1 == arg2)
        return 1;

    if ((arg1.numerator > 0) && (arg2.numerator < 0))
        return 1;
    else
    if ((arg1.numerator < 0) && (arg2.numerator > 0))
        return 0;

    arg1.make_canonical();
    arg2.make_canonical();

    tmp = arg1 - arg2;

    if (tmp.numerator > 0)
        return 1;
    else
        return 0;
}

int Rational_number::operator >= (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this >= arg1);
}

int operator >= (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg >= arg2);
}

int Rational_number::operator <= (const Rational_number &arg)const
{
    Rational_number arg1(*this), arg2(arg), tmp;

    if (arg1 == arg2)
        return 1;

    if ((arg1.numerator < 0) && (arg2.numerator > 0))
        return 1;
    else
    if ((arg1.numerator > 0) && (arg2.numerator < 0))
        return 0;

    arg1.make_canonical();
    arg2.make_canonical();

    tmp = arg1 - arg2;

    if (tmp.numerator < 0)
        return 1;
    else
        return 0;
}

int Rational_number::operator <= (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this <= arg1);
}

int operator <= (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg <= arg2);
}

Rational_number Rational_number::operator + (const Rational_number &arg)const
{
    if (*this == 0)
        return arg;
    else
    if (arg == 0)
        return *this;

    Rational_number tmp;

    tmp.numerator = (this->numerator * arg.denominator) + (this->denominator * arg.numerator);
    tmp.denominator = this->denominator * arg.denominator;

    if (tmp.numerator == 0)
        tmp.denominator = 0;

    tmp.make_canonical();

    return (tmp); 
}

Rational_number Rational_number::operator + (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this + arg1);
}

Rational_number operator + (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg + arg2);
}

Rational_number Rational_number::operator ++ ()
{
    Rational_number arg(1);

    return (*this = *this + arg);
}

Rational_number Rational_number::operator -- ()
{
    Rational_number arg(1);

    return (*this = *this - arg);
}

Rational_number Rational_number::operator - (const Rational_number &arg)const
{
    if (*this == 0)
    {
        Rational_number tmp(arg);

        tmp.numerator = -1 * tmp.numerator;

        return (tmp);
    }
    else
    if (arg == 0)
        return *this;


    Rational_number tmp;

    tmp.numerator = (this->numerator * arg.denominator) - (this->denominator * arg.numerator);
    tmp.denominator = this->denominator * arg.denominator;

    if (tmp.numerator == 0)
        tmp.denominator = 0;

    tmp.make_canonical();

    return (tmp); 
}

Rational_number Rational_number::operator - (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this - arg1);
}

Rational_number operator - (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg - arg2);
}

Rational_number Rational_number::operator * (const Rational_number &arg)const
{
    Rational_number tmp;

    tmp.numerator = this->numerator * arg.numerator;
    tmp.denominator = this->denominator * arg.denominator;

    tmp.make_canonical();

    return (tmp);
}

Rational_number Rational_number::operator * (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this * arg1);
}

Rational_number operator * (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg * arg2);
}

Rational_number Rational_number::operator / (const Rational_number &arg)const
{
    Rational_number tmp;

    if (arg == 0)
        throw Exception ("Division by zero.");

    tmp.numerator = this->numerator * arg.denominator;
    tmp.denominator = this->denominator * arg.numerator;

    if (tmp.denominator < 0)
    {
        tmp.denominator = -1 * tmp.denominator;
        tmp.numerator = -1 * tmp.numerator;
    }

    tmp.make_canonical();

    return (tmp);
}

Rational_number Rational_number::operator / (const long int arg)const
{
    Rational_number arg1(arg);

    return (*this / arg1);
}

Rational_number operator / (const long int arg1, const Rational_number &arg2)
{
    Rational_number arg(arg1);

    return (arg / arg2);
}

Rational_number::Rational_number()
{
    numerator = 0;
    denominator = 0;

    return;
}

Rational_number::Rational_number(const Big_int &arg)
{
    numerator = arg;

    if (arg != 0)
        denominator = 1;
    else
        denominator = 0;

    return;
}

Rational_number::Rational_number(const Rational_number &arg)
{
    numerator = arg.numerator;
    denominator = arg.denominator;

    return;
}

Rational_number::Rational_number(const long int arg)
{
    numerator = arg;

    if (arg != 0)
        denominator = 1;
    else
        denominator = 0;

    return;
}

Rational_number::Rational_number(const char *arg1, const char *arg2)
{
    Big_int num(arg1), den(arg2);

    numerator = num;

    if (num == 0)
        denominator = 0;
    else
    if (den == 0)
        throw Exception ("Division by zero.");
    else
        denominator = den;

    if (denominator < 0)
    {
        numerator = -1 * numerator;
        denominator = -1 * denominator;
    }

    return;
}

Rational_number::Rational_number(const char *arg)
{
    int i = 0, flag = 0;
    long int len = 0;
    char *num = NULL, *den = NULL;

    len = strlen(arg);

    if ((len == 1) && (arg[0] == '0'))
    {
        num = (char *) realloc (num, 2 * sizeof(char));
        den = (char *) realloc (den, 2 * sizeof(char));

        num[0] = '0';
        num[1] = '\0';

        den[0] = '0';
        den[1] = '\0';

        Big_int num1(num), den1(den);

        numerator = num1;
        denominator = den1;

        if (num != NULL)
            free(num);
        if (den != NULL)
            free(den);

        num = NULL;
        den = NULL;

        return;
    }
    else
    {
        if (arg[0] == '-')
        {
            num = (char *) realloc (num, (i + 1) * sizeof(char));
            num[0] = '-';

            for (i = 1; i < len; i++)
            {
                if (arg[i] == '/')
                {
                    flag = i;
                    break;
                }

                num = (char *) realloc (num, (i + 1) * sizeof(char));
                num[i] = arg[i];
            }
        }
        else
            for (i = 0; i < len; i++)
            {
                if (arg[i] == '/')
                {
                    flag = i;
                    break;
                }

                num = (char *) realloc (num, (i + 1) * sizeof(char));
                num[i] = arg[i];
            }

        if (flag != 0)
        {
            num = (char *) realloc (num, (flag + 1) * sizeof(char));
            num[flag] = '\0';

            den = (char *) realloc (den, (len - flag) * sizeof(char));

            for (i = flag + 1; i < len; i++)
                den[i - flag - 1] = arg[i];

            den[len - flag - 1] = '\0';
        }
        else
        {
            num = (char *) realloc (num, (len + 1) * sizeof(char));
            num[len] = '\0';

            den = (char *) realloc (den, 2 * sizeof(char));
            den[0] = '1';
            den[1] = '\0';
        }
    }

    Big_int num1(num), den1(den);

    if (den1 == 0)
        throw Exception("Division by zero.");

    numerator = num1;
    denominator = den1;

    if (num != NULL)
        free(num);
    if (den != NULL)
        free(den);

    num = NULL;
    den = NULL;

    return;
}

void Rational_number::make_canonical()
{
   Big_int num(numerator), den(denominator);

    if ((num == den) && (num == 0))
        return;

    if (num < 0)
        num = -1 * num;

    while (num != den)
        if (num > den)
            num = num - den;
        else
            den = den - num;

    numerator = numerator / num;
    denominator = denominator / den;

    return;
}

void Rational_number::show(FILE *f)const
{
    numerator.show(f);
    if ((denominator != 1) && (denominator != 0))
    {
        fprintf(f, "/");
        denominator.show(f);
    }
    fprintf(f, "\n");

    return;
}

void Rational_number::round()
{
    if (*this == 0)
        return;

    Rational_number tmp, half("1/2");
    tmp.numerator = numerator % denominator;
    tmp.denominator = denominator;

    if (tmp > half)
        numerator = numerator + (tmp.denominator - tmp.numerator);
    else
        numerator = numerator - tmp.numerator;

    if (numerator == 0)
        denominator = 0;

    return;
}

void Rational_number::floor()
{
    if (*this == 0)
        return;

    numerator = numerator / denominator;

    if (numerator == 0)
        denominator = 0;
    else
        denominator = 1;

    return;
}

short Rational_number::rattoshort ()
{
    if (*this == 0)
        return 0;

    if (denominator != 1)
        throw Exception_rattoshort(*this);

    return numerator.bigtoshort();
}

int Rational_number::rattoint ()
{
    if (*this == 0)
        return 0;

    if (denominator != 1)
        throw Exception_rattoint(*this);

    return numerator.bigtoint();
}

long int Rational_number::rattolong ()
{
    if (*this == 0)
        return 0;

    if (denominator != 1)
        throw Exception_rattolong(*this);

    return numerator.bigtolong();
}