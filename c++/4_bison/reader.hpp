#ifndef Reader_hpp
#define Reader_hpp

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include <vector>
#include <string>
#include <set>
#include <fstream>

class Grammar_Storage
{
    private:
        std::vector < std::vector <std::string> > rule_storage;
        std::vector < std::vector < std::vector <std::string> > > alternative_storage;
        std::set <std::string> term;
        std::set <std::string> nonterm;

    public:
        void push_rule (std::vector <std::string> v);
        void push_alternative (std::vector < std::vector <std::string> > v);
        void push_term (std::string s);
        void make_term ();
        void make_nonterm ();
        void del_bad_rules (std::set <std::string> s);
        std::vector <std::vector <std::string> > new_alternative;
        void del_eps (std::set <std::string> &s, std::vector <std::string> &v);

        std::set <std::string> make_first (std::string &s, std::set <std::string> &checked);
        std::set <std::string> make_follow (std::string &s, std::set <std::string> &checked);
        void recursive_descent ();

        void make_canonical ();

        void show ();
};

class Reader
{
    private:
        std::ifstream file_name;
        int letter;
        int str_num;
        int col_num;

    public:
        Reader (const char *s);
        ~Reader ();
        
        int getletter();
        void clear_comments ();
        Grammar_Storage get_grammar ();
};

#endif