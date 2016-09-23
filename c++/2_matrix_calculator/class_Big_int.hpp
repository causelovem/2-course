#ifndef Big_int_h
#define Big_int_h

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

class Big_int      /*just big integers class, stored in revers form*/
{
    private:
        char *number;
        int sign;
        long int num_len;

    public:
        Big_int operator = (const Big_int &arg);
        Big_int operator = (const long int arg);

        int operator == (const Big_int &arg)const;
        int operator == (const long int arg)const;
        friend int operator == (const long int arg1, const Big_int &arg2);

        int operator != (const Big_int &arg)const;
        int operator != (const long int arg)const;
        friend int operator != (const long int arg1, const Big_int &arg2);

        int operator > (const Big_int &arg)const;
        int operator > (const long int arg)const;
        friend int operator > (const long int arg1, const Big_int &arg2);

        int operator < (const Big_int &arg)const;
        int operator < (const long int arg)const;
        friend int operator < (const long int arg1, const Big_int &arg2);

        int operator >= (const Big_int &arg)const;
        int operator >= (const long int arg)const;
        friend int operator >= (const long int arg1, const Big_int &arg2);

        int operator <= (const Big_int &arg)const;
        int operator <= (const long int arg)const;
        friend int operator <= (const long int arg1, const Big_int &arg2);

        Big_int operator + (const Big_int &arg)const;
        Big_int operator + (const long int arg)const;
        friend Big_int operator + (const long int arg1, const Big_int &arg2);

        Big_int operator - (const Big_int &arg)const;
        Big_int operator - (const long int arg)const;
        friend Big_int operator - (const long int arg1, const Big_int &arg2);

        Big_int operator * (const Big_int &arg)const;
        Big_int operator * (const long int arg)const;
        friend Big_int operator * (const long int arg1, const Big_int &arg2);

        Big_int operator / (const Big_int &arg)const;
        Big_int operator / (const long int arg)const;
        friend Big_int operator / (const long int arg1, const Big_int &arg2);

        Big_int operator % (const Big_int &arg)const;
        Big_int operator % (const long int arg)const;
        friend Big_int operator % (const long int arg1, const Big_int &arg2);

        Big_int operator ++ ();
        Big_int operator -- ();

        void show(FILE *f)const;

        short bigtoshort ();
        int bigtoint ();
        long int bigtolong ();

        Big_int();
        Big_int(const Big_int &arg);
        Big_int(const long int arg);
        Big_int(const char *arg);
        ~Big_int();    
};

#endif