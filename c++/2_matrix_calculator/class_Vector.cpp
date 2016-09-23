#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Exception.hpp"
#include "class_Big_int.hpp"
#include "class_Rational_number.hpp"
#include "class_Vector.hpp"

Vector Vector::operator = (const Vector &arg)
{
    if (vec != NULL)
        delete [] vec;

    vec_len = arg.vec_len;

    vector_len = arg.vector_len;

    if (arg.vec != NULL)
        vec = new Dictionary_vector [vec_len];
    else
        vec = NULL;

    for (int i = 0; i < vec_len; i++)
    {
        vec[i].position = arg.vec[i].position;
        vec[i].value = arg.vec[i].value;
    }

    return *this;
}

Vector Vector::operator + (const Vector &arg)const
{

    if (this->vector_len != arg.vector_len)
        throw Exception ("Lengths of vectors are not equal.");

    Vector tmp, res;
    Rational_number num1, num2;
    Big_int *mas, *mas1, tmp_int;
    int flag = 0, zeroflag = 0, i = 0, j = 0;
    long int len = 0;

    mas = new Big_int [vec_len + arg.vec_len];

    for (i = 0; i < vec_len; i++)
        mas[i] = vec[i].position;

    for (i = vec_len; i < vec_len + arg.vec_len; i++)
    {
        for (j = 0; j < vec_len; j++)
            if (mas[j] == arg.vec[i - vec_len].position)
                flag = 1;

        if (flag == 1)
        {
            flag = 0;
            continue;
        }

        mas[i] = arg.vec[i - vec_len].position;
        if (mas[i] == 0)
            zeroflag = 1;
    }

    for (i = 0; i < vec_len + arg.vec_len - 1; i++)
        for (j = i; j < vec_len + arg.vec_len; j++)
            if (mas[i] < mas[j])
            {
                tmp_int = mas[i];
                mas[i] = mas[j];
                mas[j] = tmp_int;
            }

    for (i = 0; i < vec_len + arg.vec_len; i++)
    {   
        if (i == vec_len + arg.vec_len - 1)
        {
            len = i + 1;
            break;
        }

        if (mas[i] == 0)
        {
            if (zeroflag == 0)
            {
                len = i + 1;
                break;
            }
            else
            {
                len = i + 2;
                break;
            }
        }
    }

    mas1 = new Big_int [len];

    for (i = 0; i < len; i++)
        mas1[i] = mas[len - 1 - i];

    if (mas != NULL)
        delete [] mas;

    tmp.vec_len = len;
    tmp.vector_len = vector_len;

    if (tmp.vec != NULL)
        delete [] tmp.vec;

    tmp.vec = new Dictionary_vector [len];

    for (i = 0; i < len; i++)
    {
        j = 0;

        tmp.vec[i].position = mas1[i].bigtolong();

        for (j = 0; j < vec_len; j++)
            if (vec[j].position == mas1[i])
            {
                num1 = vec[j].value;
                break;
            }

        j = 0;

        for (j = 0; j < arg.vec_len; j++)
            if (arg.vec[j].position == mas1[i])
            {
                num2 = arg.vec[j].value;
                break;
            }

        tmp.vec[i].value = num1 + num2;

        num1 = 0;
        num2 = 0;
    }

    if (mas1 != NULL)
        delete [] mas1;

    for (i = 0; i < tmp.vec_len; i++)
        if (tmp.vec[i].value == 0)
        {
            for (j = i; j < vec_len - 1; j++)
            {
                tmp.vec[j].position = tmp.vec[j + 1].position;
                tmp.vec[j].value = tmp.vec[j + 1].value;
            }

            tmp.vec[vec_len - 1].value = 0;
        }

    for (i = 0; i < tmp.vec_len; i++)
    {
        if (tmp.vec[i].value == 0)
        {
            if (i == 0)
            {
                len = 0;
                break;
            }

            len = i;
            break;
        }
    }

    res.vec_len = len;
    res.vector_len = vector_len;

    if (res.vec != NULL)
        delete [] res.vec;

    if (len != 0)
        res.vec = new Dictionary_vector [len];

    for (i = 0; i < len; i++)
    {
        res.vec[i].position = tmp.vec[i].position;
        res.vec[i].value = tmp.vec[i].value;
    }

    return res;
}

Vector Vector::operator - (const Vector &arg)const
{

    if (this->vector_len != arg.vector_len)
        throw Exception ("Lengths of vectors are not equal.");

    Vector tmp, res;
    Rational_number num1, num2;
    Big_int *mas, *mas1, tmp_int;
    int flag = 0, zeroflag = 0, i = 0, j = 0;
    long int len = 0;

    mas = new Big_int [vec_len + arg.vec_len];

    for (i = 0; i < vec_len; i++)
        mas[i] = vec[i].position;

    for (i = vec_len; i < vec_len + arg.vec_len; i++)
    {
        for (j = 0; j < vec_len; j++)
            if (mas[j] == arg.vec[i - vec_len].position)
                flag = 1;

        if (flag == 1)
        {
            flag = 0;
            continue;
        }

        mas[i] = arg.vec[i - vec_len].position;
        if (mas[i] == 0)
            zeroflag = 1;
    }

    for (i = 0; i < vec_len + arg.vec_len - 1; i++)
        for (j = i; j < vec_len + arg.vec_len; j++)
            if (mas[i] < mas[j])
            {
                tmp_int = mas[i];
                mas[i] = mas[j];
                mas[j] = tmp_int;
            }

    for (i = 0; i < vec_len + arg.vec_len; i++)
    {   
        if (i == vec_len + arg.vec_len - 1)
        {
            len = i + 1;
            break;
        }

        if (mas[i] == 0)
        {
            if (zeroflag == 0)
            {
                len = i + 1;
                break;
            }
            else
            {
                len = i + 2;
                break;
            }
        }
    }

    mas1 = new Big_int [len];

    for (i = 0; i < len; i++)
        mas1[i] = mas[len - 1 - i];

    if (mas != NULL)
        delete [] mas;

    tmp.vec_len = len;
    tmp.vector_len = vector_len;

    if (tmp.vec != NULL)
        delete [] tmp.vec;

    tmp.vec = new Dictionary_vector [len];

    for (i = 0; i < len; i++)
    {
        j = 0;

        tmp.vec[i].position = mas1[i].bigtolong();

        for (j = 0; j < vec_len; j++)
            if (vec[j].position == mas1[i])
            {
                num1 = vec[j].value;
                break;
            }

        j = 0;

        for (j = 0; j < arg.vec_len; j++)
            if (arg.vec[j].position == mas1[i])
            {
                num2 = arg.vec[j].value;
                break;
            }

        tmp.vec[i].value = num1 - num2;

        num1 = 0;
        num2 = 0;
    }

    if (mas1 != NULL)
        delete [] mas1;

    for (i = 0; i < tmp.vec_len; i++)
        if (tmp.vec[i].value == 0)
        {
            for (j = i; j < vec_len - 1; j++)
            {
                tmp.vec[j].position = tmp.vec[j + 1].position;
                tmp.vec[j].value = tmp.vec[j + 1].value;
            }

            tmp.vec[vec_len - 1].value = 0;
        }

    for (i = 0; i < tmp.vec_len; i++)
    {
        if (tmp.vec[i].value == 0)
        {
            if (i == 0)
            {
                len = 0;
                break;
            }

            len = i;
            break;
        }
    }

    res.vec_len = len;
    res.vector_len = vector_len;

    if (res.vec != NULL)
        delete [] res.vec;

    if (len != 0)
        res.vec = new Dictionary_vector [len];

    for (i = 0; i < len; i++)
    {
        res.vec[i].position = tmp.vec[i].position;
        res.vec[i].value = tmp.vec[i].value;
    }

    return res;
}

Vector Vector::operator - ()
{
    for (int i = 0; i < vec_len; i++)
        vec[i].value = -1 * vec[i].value;

    return *this;
}

Vector Vector::operator * (const Rational_number &arg)const
{
    Vector tmp(*this);

    if (arg == 0)
    {
        if (tmp.vec != NULL)
            delete [] tmp.vec;

        tmp.vec = NULL;

        tmp.vec_len = 0;

        return tmp;
    }

    for (int i = 0; i < vec_len; i++)
        tmp.vec[i].value = arg * vec[i].value;

    return tmp;
}

Vector operator * (const Rational_number arg1, const Vector &arg2)
{
    
    return (arg2 * arg1);
}

Vector Vector::operator / (const Rational_number &arg)const
{
   if (arg == 0)
        throw Exception ("Division by zero.");

    Vector tmp(*this);

    for (int i = 0; i < vec_len; i++)
        tmp.vec[i].value = vec[i].value / arg;

    return tmp;
}

Rational_number Vector::operator [] (const long int arg)const
{
    if ((arg > vector_len) || (arg < 1))
        throw Exception_vector_border (vector_len, arg);

    Rational_number num;

    for (int i = 0; i < vec_len; i++)
        if (vec[i].position == arg - 1)
            return vec[i].value;

    return num;
}

Vector::Vector()
{
    vec = NULL;

    vector_len = 0;
    vec_len = 0;
    
    return;
}

Vector::Vector(const Rational_number &nuber, const int size)
{
    vector_len = size;

    if (nuber != 0)
    {
        vec_len = size;
        vec = new Dictionary_vector [vec_len];
    }
    else
    {
        vec_len = 0;
        vec = NULL;
    }

    for (int i = 0; i < vec_len; i++)
    {
        vec[i].position = i;
        vec[i].value = nuber;
    }

    return;
}

Vector::Vector(const Vector &arg)
{
    vector_len = arg.vector_len;
    vec_len = arg.vec_len;

    vec = new Dictionary_vector [vec_len];

    for (int i = 0; i < vec_len; i++)
    {
        vec[i].position = arg.vec[i].position;
        vec[i].value = arg.vec[i].value;
    }

    return;
}

Vector::Vector(const char *file)
{
    FILE *f;
    long int coord = 0;
    int letter = 0, chsz = 0;;
    char *check = NULL;

    if ((f = fopen(file, "r")) == NULL)
        throw Exception_file_name (file);

    letter = getc(f);

    while ((letter != EOF) && ((letter == '\n') || (letter == '#')))
    {
        if (letter == '#')
        {
            while (letter != '\n')
                letter = getc(f);
            letter = getc(f);
            continue;
        }

        if (letter == '\n')
            letter = getc(f);
    }

    if (letter == EOF)
    {
        printf(">Vector was not founded in this file.\n");

        vec = NULL;

        vector_len = 0;
        vec_len = 0;

        fclose(f);
        return;
    }

    while (letter != ' ')
    {
        check = (char *) realloc (check, ++chsz * sizeof(char));
        check[chsz - 1] = letter;
        if ((letter = getc(f)) == EOF)
        {
            printf(">Vector was not founded in this file.\n");

            if (check != NULL)
                free(check);
            check = NULL;

            vec = NULL;

            vector_len = 0;
            vec_len = 0;
            fclose(f);
            return;
        }
    }
    check = (char *) realloc (check, ++chsz * sizeof(char));
    check[chsz - 1] = '\0';

    if (strcmp(check, "Vector") == 0)
    {
        fscanf(f, "%li", &vector_len);

        vec_len = 0;

        vec = NULL;

        letter = getc(f);
        while (letter != EOF)
        {
            if (letter == '#')
            {
                while (letter != '\n')
                    letter = getc(f);
                letter = getc(f);
                continue;
            }

            coord = 0;
            fscanf(f, "%li", &coord);

            if (check != NULL)
                free(check);
            check = NULL;
            chsz = 0;

            letter = getc(f);
            if (letter == ' ')
                letter = getc(f);

            if (coord != 0)
            {
                while ((letter != '\n') && (letter != EOF))
                {
                    check = (char *) realloc (check, ++chsz * sizeof(char));
                    check[chsz - 1] = letter;
                    letter = getc(f);
                }

                check = (char *) realloc (check, ++chsz * sizeof(char));
                check[chsz - 1] = '\0';

                this->push(Rational_number(check), coord);

                if (letter == EOF)
                    break;
            }
            else
            {
                while ((letter != '\n') && (letter != EOF))
                    letter = getc(f);
            }
        }
        if (check != NULL)
            free(check);
        check = NULL;
    }
    else
    {
        printf(">Vector was not founded in this file.\n");

        if (check != NULL)
            free(check);
        check = NULL;

        vec = NULL;

        vector_len = 0;
        vec_len = 0;
        fclose(f);
        return;
    }

    fclose(f);

    return;
}

Vector::~Vector()
{
    if (vec != NULL)
        delete [] vec;

    vector_len = 0;
    vec_len = 0;

    return;
}

void Vector::show(FILE *f)const
{
    if (vec == NULL)
    {
        fprintf(f, "Empty vector.\n");
        return;
    }

    for (int i = 0; i < vec_len; i++)
    {
        fprintf(f, "%li   ", vec[i].position + 1);
        vec[i].value.show(f);
    }

    fprintf(f, "\n");

    return;
}

long int Vector::get_vector_len()const
{

    return (vector_len);
}

long int Vector::get_vec_len()const
{

    return (vec_len);
}

void Vector::push(const Rational_number &number, const long int arg)
{
    if ((arg > vector_len) || (arg < 1))
        throw Exception_vector_border (arg, vector_len);

    if (number == 0)
    {
        for (int i = 0; i < vec_len; i++)
            if (vec[i].position == arg - 1)
            {
                for (int j = i; j < vec_len - 1; j++)
                {
                    vec[j].position = vec[j + 1].position;
                    vec[j].value = vec[j + 1].value;
                }

                vec_len--;

                if (vec_len == 0)
                {
                    delete [] vec;
                    vec = NULL;
                }
            }

        return;
    }

    for (int i = 0; i < vec_len; i++)
        if (vec[i].position == arg - 1)
        {
            vec[i].value = number;
            return;
        }

    Vector res;

    res.vector_len = vector_len;
    res.vec_len = vec_len + 1;

    res.vec = new Dictionary_vector [res.vec_len];

    for (int i = 0; i < vec_len; i++)
    {
        res.vec[i].position = vec[i].position;
        res.vec[i].value = vec[i].value;
    }

    res.vec[res.vec_len - 1].position = arg - 1;
    res.vec[res.vec_len - 1].value = number;

    *this = res;

    return;
}

void Vector::write(const char *file)
{
    FILE *f;

    if ((f = fopen(file, "w+")) == NULL)
        throw Exception_file_name (file);

    fprintf(f, "Vector %li\n", vector_len);

    for (int i = 0; i < vec_len; i++)
    {
        fprintf(f, "%li ",vec[i].position + 1);
        vec[i].value.show(f);
    }

    fclose(f);

    return;
}