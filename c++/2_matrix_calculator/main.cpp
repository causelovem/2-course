#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <climits>

#include "class_Exception.hpp"
#include "class_Big_int.hpp"
#include "class_Rational_number.hpp"
#include "class_Vector.hpp"
#include "class_Matrix.hpp"
#include "generator.hpp"

using namespace std;


int main ()
{
    try
    {
        srand((unsigned int)time(0));

        double x,y;
        x=(float)clock();

        Matrix mat1("mat_file1"), mat2("mat_file2"), mat3("mat_file3"), mat4;
        Big_int num1("1314"), num2("5612");
        Rational_number rat1("1"), rat2, rat3(num1);
        long int sum = 0;

        Row_position rp1(1), rp2(2), rp3(3);
        Col_position cp1(1), cp2(2), cp3(3);

        mat4 = mat2[cp2] * mat3[rp2];

        mat4.show(stdout);


        



        y=(float)clock();
        x=(float)(y-x)/CLOCKS_PER_SEC;
        cout << x << endl;
    }

    catch(Exception &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_vector_border &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_bigtoshort &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_bigtoint &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_bigtolong &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_rattoshort &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_rattoint &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_rattolong &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_file_name &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_matrix_border &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(Exception_matrix_sizes &err)
    {
        err.show(stdout);

        return -1;
    }

    catch(...)
    {
        fprintf(stdout, ">Something unknown catched...\n");

        return -1;
    }



    return 0;
}