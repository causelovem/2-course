#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include <vector>
#include <string>

#include "reader.hpp"

using namespace std;

int main ()
{
    try
    {
        Reader R("graph_file_format.ypp");   /*graph_file_format.ypp*/

        Grammar_Storage storage;

        storage = R.get_grammar();

        storage.show();

        storage.make_canonical();

        storage.recursive_descent();
    }
    catch (const char *s)
    {
        cerr << s << '\n';

        return -1;
    }

    return 0;
}

