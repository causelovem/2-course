#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Exception.hpp"
#include "class_Big_int.hpp"

using namespace std;

Big_int Big_int::operator = (const Big_int &arg)
{
    if (this == &arg)
        return *this;

    if (number != NULL)
        free(number);

    number = NULL;

    num_len = arg.num_len;
    sign = arg.sign;

    number = (char *) calloc (num_len, sizeof(char));

    for (int i = 0; i < num_len; i++)
        number[i] = arg.number[i];

    return *this;
}

Big_int Big_int::operator = (const long int arg)
{
    Big_int tmp(arg);

    return (*this = tmp);
}

int Big_int::operator == (const Big_int &arg)const
{
    if ((this->sign != arg.sign) || (this->num_len != arg.num_len))
        return 0;

    for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] != arg.number[i])
                return 0;

    return 1;
}

int Big_int::operator == (const long int arg)const
{
    Big_int arg1(arg);

    return (*this == arg1);
}

int operator == (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg == arg2);
}

int Big_int::operator != (const Big_int &arg)const
{
    if ((this->sign != arg.sign) || (this->num_len != arg.num_len))
        return 1;

    for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] != arg.number[i])
                return 1;

    return 0;
}

int Big_int::operator != (const long int arg)const
{
    Big_int arg1(arg);

    return (*this != arg1);
}

int operator != (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg != arg2);
}

int Big_int::operator > (const Big_int &arg)const
{
    if ((this->sign < arg.sign) || (this->num_len > arg.num_len))
        return 1;
    else
    if ((this->sign > arg.sign) || (this->num_len < arg.num_len))
        return 0;

    if (arg.sign == 0)
    {
        for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] > arg.number[i])
                return 1;
            else
            if (this->number[i] < arg.number[i])
                return 0;

        return 0;
    }

    if (arg.sign == 1)
    {
        for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] < arg.number[i])
                return 1;
            else
            if (this->number[i] > arg.number[i])
                return 0;

        return 0;
    }
}

int Big_int::operator > (const long int arg)const
{
    Big_int arg1(arg);

    return (*this > arg1);
}

int operator > (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg > arg2);
}

int Big_int::operator < (const Big_int &arg)const
{
    if ((this->sign > arg.sign) || (this->num_len < arg.num_len))
        return 1;
    else
    if ((this->sign < arg.sign) || (this->num_len > arg.num_len))
        return 0;

    if (arg.sign == 0)
    {
        for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] < arg.number[i])
                return 1;
            else
            if (this->number[i] > arg.number[i])
                return 0;

        return 0;
    }

    if (arg.sign == 1)
    {
        for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] > arg.number[i])
                return 1;
            else
            if (this->number[i] < arg.number[i])
                return 0;

        return 0;
    }
}

int Big_int::operator < (const long int arg)const
{
    Big_int arg1(arg);

    return (*this < arg1);
}

int operator < (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg < arg2);
} 

int Big_int::operator >= (const Big_int &arg)const
{
    if (*this == arg)
        return 1;

    if ((this->sign < arg.sign) || (this->num_len > arg.num_len))
        return 1;
    else
    if ((this->sign > arg.sign) || (this->num_len < arg.num_len))
        return 0;

    if (arg.sign == 0)
    {
        for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] > arg.number[i])
                return 1;
            else
            if (this->number[i] < arg.number[i])
                return 0;

        return 0;
    }

    if (arg.sign == 1)
    {
        for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] < arg.number[i])
                return 1;
            else
            if (this->number[i] > arg.number[i])
                return 0;

        return 0;
    }
}

int Big_int::operator >= (const long int arg)const
{
    Big_int arg1(arg);

    return (*this >= arg1);
}

int operator >= (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg >= arg2);
}

int Big_int::operator <= (const Big_int &arg)const
{
    if (*this == arg)
        return 1;

    if ((this->sign > arg.sign) || (this->num_len < arg.num_len))
        return 1;
    else
    if ((this->sign < arg.sign) || (this->num_len > arg.num_len))
        return 0;

    if (arg.sign == 0)
    {
        for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] < arg.number[i])
                return 1;
            else
            if (this->number[i] > arg.number[i])
                return 0;

        return 0;
    }

    if (arg.sign == 1)
    {
        for (int i = arg.num_len - 1; i > -1; i--)
            if (this->number[i] > arg.number[i])
                return 1;
            else
            if (this->number[i] < arg.number[i])
                return 0;

        return 0;
    }
}

int Big_int::operator <= (const long int arg)const
{
    Big_int arg1(arg);

    return (*this <= arg1);
}

int operator <= (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg <= arg2);
}

Big_int Big_int::operator + (const Big_int &arg)const
{
    Big_int tmp, arg1(*this), arg2(arg);

    long int tmp_len = 0;

    int digit = 0, tmp_digit = 0;

    if ((arg1.sign == 1) && (arg2.sign == 0))
    {
        arg1.sign = 0;
        return (arg2 - arg1);
    }
    else
    if ((arg1.sign == 0) && (arg2.sign == 1))
    {
        arg2.sign = 0;
        return (arg1 - arg2);
    }

    if ((arg1 == arg2) && (arg1 == 0))
        return (tmp);

    if ((arg1.sign == 0) && (arg2.sign == 0))
        tmp.sign = 0;
    else
    if ((arg1.sign == 1) && (arg2.sign == 1))
        tmp.sign = 1;

    if (arg1.num_len > arg2.num_len)
    {
        tmp_len = this->num_len + 1;

        arg2.number = (char *) realloc (arg2.number, tmp_len * sizeof(char));
        for (int i = arg.num_len; i < tmp_len; i++)
            arg2.number[i] = 0;
    }
    else
    if (arg1.num_len < arg2.num_len)
    {
        tmp_len = arg.num_len + 1;

        arg1.number = (char *) realloc (arg1.number, tmp_len * sizeof(char));
        for (int i = this->num_len; i < tmp_len; i++)
            arg1.number[i] = 0;
    }
    else
    if (arg1.num_len == arg2.num_len)
        tmp_len = this->num_len + 1;

    tmp.number = (char *) realloc (tmp.number, tmp_len * sizeof(char));

    for (int i = 0; i < tmp_len - 1; i++)
    {
        digit = arg1.number[i] + arg2.number[i] + tmp_digit;

        tmp.number[i] = digit % 10;

        tmp_digit = digit / 10;
    }

    if (tmp_digit == 1)
        tmp.number[tmp_len - 1] = 1;
    else
        tmp_len--;

    tmp.num_len = tmp_len;

    return (tmp);
}

Big_int Big_int::operator + (const long int arg)const
{
    Big_int arg1(arg);

    return (*this + arg1);
}

Big_int operator + (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg + arg2);
}

Big_int Big_int::operator ++ ()
{
    Big_int arg(1);

    return (*this = *this + arg);
}

Big_int Big_int::operator - (const Big_int &arg)const
{
    Big_int tmp, tmp1, arg1(*this), arg2(arg);

    long int tmp_len = 0;

    int digit = 0, tmp_digit = 0;

    if ((arg1.sign == 0) && (arg2.sign == 1))
    {
        arg2.sign = 0;
        return (arg1 + arg2);
    }
    else
    if ((arg1.sign == 1) && (arg2.sign == 0))
    {
        arg2.sign = 1;
        return (arg1 + arg2);
    }

    if (((arg1.sign == 0) && (arg2.sign == 0) && (arg1 == arg2)) || ((arg1.sign == 1) && (arg1.sign == 1) && (arg1 == arg2)))
        return (tmp);

    if (arg1 == 0)
    {
        arg2.sign = 1;
        return arg2;
    }
    else
    if (arg2 == 0)
        return arg1;

    if ((arg1.sign == 0) && (arg2.sign == 0))
    {
        if (arg2 > arg1)
        {
            tmp1 = arg1;
            arg1 = arg2;
            arg2 = tmp1;

            tmp.sign = 1;
        }
        else
            tmp.sign = 0;
    }
    else
    if ((arg1.sign == 1) && (arg2.sign == 1))
    {
        if (arg2 > arg1)
        {
            tmp1 = arg1;
            arg1 = arg2;
            arg2 = tmp1;

            tmp.sign = 0;
        }
        else
            tmp.sign = 1;
    }

    if (arg1.num_len > arg2.num_len)
    {
        tmp_len = arg1.num_len;

        arg2.number = (char *) realloc (arg2.number, tmp_len * sizeof(char));
        for (int i = arg.num_len; i < tmp_len; i++)
            arg2.number[i] = 0;
    }
    else
    if (arg1.num_len == arg2.num_len)
            tmp_len = arg1.num_len;

    tmp.number = (char *) realloc (tmp.number, tmp_len * sizeof(char));

    for (int i = 0; i < tmp_len; i++)
    {
        digit = arg1.number[i] - arg2.number[i];

        if ((digit < 0) && (i != tmp_len - 1))
        {
            arg1.number[i + 1]--;
            digit = digit + 10;
        }

        tmp.number[i] = digit;
    }

    for (int i = tmp_len - 1; i > -1; i--)
        if (tmp.number[i] != 0)
            break;
        else
            tmp_len--;

    tmp.num_len = tmp_len;

    return (tmp);
}

Big_int Big_int::operator - (const long int arg)const
{
    Big_int arg1(arg);

    return (*this - arg1);
}

Big_int operator - (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg - arg2);
}

Big_int Big_int::operator -- ()
{
    Big_int arg(1);

    return (*this = *this - arg);
}

Big_int Big_int::operator * (const Big_int &arg)const
{
    Big_int tmp, tmp1, arg1(*this), arg2(arg);

    int i = 0, j = 0, digit = 0, tmp_digit = 0;

    long int tmp_len = 0;

    if ((arg1 == 0) || (arg2 == 0))
        return (tmp);
    else
    if (arg1 == 1)
        return (arg2);
    else
    if (arg2 == 1)
        return (arg1);
    else
    if (arg1 == -1)
        return (0 - arg2);
    else
    if (arg2 == -1)
        return (0 - arg1);

    if (arg2.num_len > arg1.num_len)
    {
        tmp1 = arg1;
        arg1 = arg2;
        arg2 = tmp1;
    }

    tmp_len = arg1.num_len + arg2.num_len;

    tmp1 = 0;

    tmp.num_len = tmp_len;
    tmp1.num_len = tmp_len;

    tmp.number = (char *) realloc (tmp.number, tmp_len * sizeof(char));
    tmp1.number = (char *) realloc (tmp1.number, tmp_len * sizeof(char));
    for (i = 1; i < tmp_len; i++)
    {
        tmp1.number[i] = 0;
        tmp.number[i] = 0;
    }

    for (j = 0; j < arg2.num_len; j++)
    {
        for (i = 0; i < arg1.num_len; i++)
        {
            digit = arg1.number[i] * arg2.number[j] + tmp_digit;

            tmp1.number[j + i] = digit % 10;

            tmp_digit = digit / 10;
        }

        if (tmp_digit != 0)
            tmp1.number[j + arg1.num_len] = tmp_digit;
        else
        if (j == arg2.num_len - 1)
            tmp_len--;

        tmp = tmp + tmp1;

        for (i = 0; i < tmp_len; i++)
            tmp1.number[i] = 0;

        tmp_digit = 0;
    }

    tmp.num_len = tmp_len;

    if (arg1.sign != arg2.sign)
        tmp.sign = 1;
    else
        tmp.sign = 0;

    return (tmp);
}

Big_int Big_int::operator * (const long int arg)const
{
    Big_int arg1(arg);

    return (*this * arg1);
}

Big_int operator * (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg * arg2);
}

Big_int Big_int::operator / (const Big_int &arg)const
{
    Big_int tmp, tmp1, tmp2, arg1(*this), arg2(arg);

    int i = 0, j = 0;

    long int tmp_len = 0;

    if (arg2 == 0)
        throw Exception ("Division by zero.");
    else
    if (arg1 == 0)
        return (tmp);
    else
    if (arg1 == 1)
        return (arg2);
    else
    if (arg2 == 1)
        return (arg1);
    else
    if (arg1 == -1)
        return (0 - arg2);
    else
    if (arg2 == -1)
        return (0 - arg1);

    if (arg1.sign != arg2.sign)
        tmp.sign = 1;
    else
        tmp.sign = 0;

    arg1.sign = 0;
    arg2.sign = 0;

    if (arg1 == arg2)
    {
        tmp.number[0] = 1;
        return tmp;
    }

    if (arg2 > arg1)
    {
        tmp.sign = 0;
        return (tmp);
    }

    tmp_len = arg1.num_len;

    tmp.num_len = tmp_len;
    tmp1.num_len = arg2.num_len + 1;
    tmp2.num_len = arg1.num_len;

    tmp.number = (char *) realloc (tmp.number, tmp_len * sizeof(char));
    for (i = 1; i < tmp_len; i++)
        tmp.number[i] = 0;

    tmp1.number = (char *) realloc (tmp1.number, (arg2.num_len + 1) * sizeof(char));
    for (i = 0; i < arg2.num_len + 1; i++)
        tmp1.number[i] = 0;

    tmp2.number = (char *) realloc (tmp2.number, arg1.num_len * sizeof(char));
    for (i = 0; i < arg1.num_len; i++)
        tmp2.number[i] = 0;


    while (arg1 >= arg2)
    {
        for (i = 0; i < arg2.num_len; i++)
            tmp1.number[arg2.num_len - 1 - i] = arg1.number[arg1.num_len - 1 - i];
        tmp1.num_len--;

        if (tmp1 < arg2)
        {
            tmp1.num_len++;
            for (i = arg2.num_len; i > 0; i--)
                tmp1.number[i] = tmp1.number[i - 1];
            tmp1.number[0] = arg1.number[arg1.num_len - arg2.num_len - 1];
        }

        for (i = 1; i < 11; i++)
            if (arg2 * i > tmp1)
            {
                for (j = tmp.num_len - 1; j > 0; j--)
                    tmp.number[j] = tmp.number[j - 1];
                tmp.number[0] = --i;
                break;
            }

        tmp2 = arg2 * tmp.number[0];

        /*if ((tmp2 == arg1) || (arg1 - tmp2 < arg2))
            break;*/

        j = tmp2.num_len;
        tmp2.num_len = arg1.num_len;
        tmp2.number = (char *) realloc (tmp2.number, tmp2.num_len * sizeof(char));
        for (i = j; i < tmp2.num_len; i++)
            tmp2.number[i] = 0;

        for (i = 0; i < j; i++)
        {
            tmp2.number[tmp2.num_len - 1 - i] = tmp2.number[j - 1 - i];
            if (tmp2.num_len > j)
                tmp2.number[j - 1 - i] = 0;
        }

        arg1 = arg1 - tmp2;

        if (arg1 <= 0)
            break;

        for (i = 0; i < arg2.num_len + 1; i++)
            tmp1.number[i] = 0;

        tmp1.num_len = arg2.num_len + 1;
    }

    for (i = tmp.num_len - 1; i > -1; i--)
    {
        if (tmp.number[i] != 0)
            break;
        else
            tmp.num_len--;
    }

    return (tmp);
}

Big_int Big_int::operator / (const long int arg)const
{
    Big_int arg1(arg);

    return (*this / arg1);
}

Big_int operator / (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg / arg2);
}

Big_int Big_int::operator % (const Big_int &arg)const
{
    Big_int tmp, tmp1, tmp2(1), arg1(*this), arg2(arg);

    int i = 0, j = 0, digit = 0, tmp_digit = 0;

    long int tmp_len = 0;

    if (arg2 == 0)
        throw Exception ("Division by zero.");
    else
    if (arg1 == 0)
        return (tmp);

    tmp.sign = arg1.sign;

    arg1.sign = 0;
    arg2.sign = 0;

    if (arg1 == arg2)
    {
        tmp.sign = 0;
        return (tmp);
    }

    if (arg2 > arg1)
    {
        tmp.sign = 0;
        return (arg1);
    }

    tmp_len = arg1.num_len;

    tmp.num_len = tmp_len;
    tmp1.num_len = arg2.num_len + 1;
    tmp2.num_len = arg1.num_len;

    tmp.number = (char *) realloc (tmp.number, tmp_len * sizeof(char));
    for (i = 1; i < tmp_len; i++)
        tmp.number[i] = 0;

    tmp1.number = (char *) realloc (tmp1.number, (arg2.num_len + 1) * sizeof(char));
    for (i = 0; i < arg2.num_len + 1; i++)
        tmp1.number[i] = 0;

    tmp2.number = (char *) realloc (tmp2.number, arg1.num_len * sizeof(char));
    for (i = 0; i < arg1.num_len; i++)
        tmp2.number[i] = 0;


    while (arg1 > arg2)
    {
        for (i = 0; i < arg2.num_len; i++)
            tmp1.number[arg2.num_len - 1 - i] = arg1.number[arg1.num_len - 1 - i];
        tmp1.num_len--;

        if (tmp1 < arg2)
        {
           tmp1.num_len++;
            for (i = arg2.num_len; i > 0; i--)
                tmp1.number[i] = tmp1.number[i - 1];
            tmp1.number[0] = arg1.number[arg1.num_len - arg2.num_len - 1];
        }

        for (i = 1; i < 11; i++)
            if (arg2 * i > tmp1)
            {
                for (j = tmp.num_len - 1; j > 0; j--)
                    tmp.number[j] = tmp.number[j - 1];
                tmp.number[0] = --i;
                break;
            }

        tmp2 = arg2 * tmp.number[0];
        j = tmp2.num_len;
        tmp2.num_len = arg1.num_len;
        tmp2.number = (char *) realloc (tmp2.number, tmp2.num_len * sizeof(char));
        for (i = j; i < tmp2.num_len; i++)
            tmp2.number[i] = 0;

        for (i = 0; i < j; i++)
        {
            tmp2.number[tmp2.num_len - 1 - i] = tmp2.number[j - 1 - i];
            if (tmp2.num_len > j)
                tmp2.number[j - 1 - i] = 0;
        }

        arg1 = arg1 - tmp2;

        if (arg1 <= 0)
            break;

        for (i = 0; i < arg2.num_len + 1; i++)
            tmp1.number[i] = 0;

        tmp1.num_len = arg2.num_len + 1;
    }

    return (arg1);
}

Big_int Big_int::operator % (const long int arg)const
{
    Big_int arg1(arg);

    return (*this % arg1);
}

Big_int operator % (const long int arg1, const Big_int &arg2)
{
    Big_int arg(arg1);

    return (arg % arg2);
}

Big_int::Big_int()
{
    number = NULL;
    number = (char *) calloc (1, sizeof(char));
    number[0] = 0;
    num_len = 1;
    sign = 0;

    return;
}

Big_int::Big_int(const Big_int &arg)
{
    num_len = arg.num_len;

    number = NULL;

    number = (char *) calloc (num_len + 1, sizeof(char));

    for (int i = 0; i < num_len; i++)
        number[i] = arg.number[i];

    sign = arg.sign;

    return;
}

Big_int::Big_int(const long int arg)
{
    int tmp = 0;

    long int arg1 = arg;

    if (arg1 >= 0)
        sign = 0;
    else
    if (arg1 < 0)
    {
        arg1 = -arg1;
        sign = 1;
    }

    num_len = 0;
    number = NULL;

    if (arg1 == 0)
    {
        number = (char *) calloc (1, sizeof(char));
        number[0] = 0;
        num_len = 1;
        return;
    }

    while (arg1 != 0)
    {
        tmp = arg1 % 10;
        number = (char *) realloc (number, ++num_len * sizeof(char));
        number[num_len - 1] = tmp;
        arg1 = arg1 / 10;
    }

    return;
}

Big_int::Big_int(const char *arg)
{
    num_len = strlen(arg);

    number = (char *) malloc (num_len * sizeof(char));

    if (arg[0] == '-')
    {
        for (int i = 1; i < num_len; i++)
            number[i - 1] = arg[num_len - i] - '0';

        num_len--;

        sign = 1;
    }
    else
    {
        for (int i = 0; i < num_len; i++)
            number[i] = arg[num_len - 1 - i] - '0';

        sign = 0;
    }

    return;
}

Big_int::~Big_int()
{
    if (number != NULL)
        free(number);

    number = NULL;
    num_len = 0;
    sign = 0;

    return;
}

void Big_int::show(FILE *f)const
{
    if (sign == 1)
        fprintf(f, "-");

    for (int i = num_len - 1; i > -1; i--)
        fprintf(f, "%i",number[i]);
    /*fprintf(f, "\n");*/

    return;
}

short Big_int::bigtoshort ()
{
    short res = 0;
    int k = 1;

    if (*this > SHRT_MAX)
        throw Exception_bigtoshort (*this);

    for (int i = 0; i < num_len; i++)
    {
        res = res + number[i] * k;

        k = k * 10;
    }

    if (sign == 1)
        res = -1 * res;

    return (res);
}

int Big_int::bigtoint ()
{
    int res = 0, k = 1;

    if (*this > INT_MAX)
        throw Exception_bigtoint (*this);

    for (int i = 0; i < num_len; i++)
    {
        res = res + number[i] * k;

        k = k * 10;
    }

    if (sign == 1)
        res = -1 * res;

    return (res);
}

long int Big_int::bigtolong ()
{
    long int res = 0, k = 1;

    if (*this > LONG_MAX)
        throw Exception_bigtolong (*this);

    for (int i = 0; i < num_len; i++)
    {
        res = res + number[i] * k;

        k = k * 10;
    }

    if (sign == 1)
        res = -1 * res;

    return (res);
}