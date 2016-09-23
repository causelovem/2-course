#define eps "\u03B5"
#define END "$"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include <vector>
#include <string>
#include <set>
#include <iterator>
#include <algorithm>
#include <fstream>

#include "reader.hpp"

using namespace std;

std::set <std::string> operator + (const std::set <std::string> &arg1, const std::set <std::string> &arg2)
{
    std::set <std::string> res = arg1;

    for (std::set <std::string>::iterator it = arg2.begin(); it != arg2.end(); it++)
        res.insert(*it);

    return res;
}

bool check_cross_sets (const std::set<std::string> &arg1, const std::set<std::string> &arg2)
{
    std::set <std::string> res = arg1;

    for (std::set <std::string>::iterator it = arg1.begin(); it != arg1.end(); it++)
        if (arg2.find(*it) == arg2.end())
            res.erase(*it);

    if (res.size() == 0)
        return false;
    else
        return true;

    /*return res;*/
}

void Grammar_Storage::push_rule (std::vector <string> v)
{
    rule_storage.push_back(v);
}

void Grammar_Storage::push_alternative (std::vector < std::vector <string> > v)
{
    alternative_storage.push_back(v);
}

void Grammar_Storage::push_term (std::string s)
{
    term.insert(s);
}

void Grammar_Storage::make_nonterm ()
{
    for (int i = 0; i < rule_storage.size(); i++)
        for (int j = 0; j < rule_storage[i].size(); j++)
            nonterm.insert(rule_storage[i][j]);

    for (int i = 0; i < alternative_storage.size(); i++)
        for (int j = 0; j < alternative_storage[i].size(); j++)
            for (int k = 0; k < alternative_storage[i][j].size(); k++)
                    nonterm.insert(alternative_storage[i][j][k]);

    for (std::set <std::string>::iterator it = term.begin(); it != term.end(); it++)
        nonterm.erase(*it);
}

void Grammar_Storage::make_term ()
{
    for (int i = 0; i < alternative_storage.size(); i++)
        for (int j = 0; j < alternative_storage[i].size(); j++)
            for (int k = 0; k < alternative_storage[i][j].size(); k++)
                if ((alternative_storage[i][j][k][0] == '\'') || (alternative_storage[i][j][k] == eps))
                    term.insert(alternative_storage[i][j][k]);
}

void Grammar_Storage::show ()
{
    cout << ">NOT TERMINALS\n";
    for (std::set <std::string>::iterator it = nonterm.begin(); it != nonterm.end(); it++)
        cout << *it << '\n';
    cout << '\n';

    cout << ">TERMINALS\n";
    for (std::set <std::string>::iterator it = term.begin(); it != term.end(); it++)
        cout << *it << '\n';
    cout << '\n';

    for (int i = 0; i < rule_storage.size(); i++)
    {
        for (int j = 0; j < rule_storage[i].size(); j++)
            cout << rule_storage[i][j];
            /*cout << rule_storage[i][j] << ' ';*/
        cout << ": ";

        for (int j = 0; j < alternative_storage[i].size(); j++)
        {
            for (int k = 0; k < alternative_storage[i][j].size(); k++)
                cout << alternative_storage[i][j][k] << ' ';

            if (j != alternative_storage[i].size() - 1)
                cout << "\n| ";
            else
                cout << "\n\n";
        }
    }
}

void Grammar_Storage::del_bad_rules (std::set <std::string> s)
{
    for (int i = 0; i < rule_storage.size(); i++)
        for (int j = 0; j < rule_storage[i].size(); j++)
            if (s.find(rule_storage[i][j]) != s.end()) // NOTREACH || NOTGEN
            {
                rule_storage.erase(rule_storage.begin() + i);
                alternative_storage.erase(alternative_storage.begin() + i);
                i--;
                break;
            }

    for (int i = 0; i < alternative_storage.size(); i++)
    {
        for (int j = 0; j < alternative_storage[i].size(); j++)
        {
            for (int k = 0; k < alternative_storage[i][j].size(); k++)
            {
                if (s.find(alternative_storage[i][j][k]) != s.end()) // NOTREACH || NOTGEN
                {
                    alternative_storage[i].erase(alternative_storage[i].begin() + j);
                    j--;
                    break;
                }
            }

            if (alternative_storage[i].empty() == true)
            {
                rule_storage.erase(rule_storage.begin() + i);
                alternative_storage.erase(alternative_storage.begin() + i);
                i--;
            }
        }
    }

    term.clear();

    for (int i = 0; i < alternative_storage.size(); i++)
        for (int j = 0; j < alternative_storage[i].size(); j++)
            for (int k = 0; k < alternative_storage[i][j].size(); k++)
                if (nonterm.find(alternative_storage[i][j][k]) == nonterm.end()) // OUT OF NONTERM
                    term.insert(alternative_storage[i][j][k]);

    for (std::set <std::string>::iterator it = s.begin(); it != s.end(); it++)
        nonterm.erase(*it);


    /*for (std::set <std::string>::iterator it = term.begin(); it != term.end(); it++)
        if (((*it)[0] != '\'') && ((*it) != eps))
            tmp_term.insert(*it);

    term.clear();
    make_term();

    for (std::set <std::string>::iterator it = tmp_term.begin(); it != tmp_term.end(); it++)
        term.insert(*it);*/

    show();
}

void Grammar_Storage::del_eps (std::set <std::string> &s, std::vector <std::string> &v)
{
    bool inflag = false;
    std::vector <std::string> tv;

    if ((s.empty() == true) || ((v.size() == 1)))
        return;

    for (int i = 0; i < v.size(); i++)
        if (s.find(v[i]) != s.end())
        {
            inflag = true;
            break;
        }

    if (inflag == false)
        return;

    inflag = false;

    for (int i = 0; i < v.size(); i++)
        if (s.find(v[i]) != s.end())
        {
            tv = v;
            tv.erase(tv.begin() + i);
            new_alternative.push_back(tv);
            del_eps(s, tv);
        }

    return;
}

void Grammar_Storage::make_canonical ()
{
    std::set <std::string> gen, not_gen, reach, not_reach, empty;
    std::vector<std::string> tmp_alt;
    bool inflag = false, changeflag = false;


    /*DELETE NOT GENERATING NONTERMS*/

    for (int i = 0; i < alternative_storage.size(); i++)
    {
        for (int j = 0; j < alternative_storage[i].size(); j++)
        {
            for (int k = 0; k < alternative_storage[i][j].size(); k++)
            {
                if (nonterm.find(alternative_storage[i][j][k]) != nonterm.end()) // NONTERM
                {
                    inflag = true;
                    break;
                }
            }

            if (inflag == false)
                for (int t = 0; t < rule_storage[i].size(); t++)
                    gen.insert(rule_storage[i][t]);
            else
                inflag = false;
        }
    }

    inflag = false;

    while (1)
    {
        for (int i = 0; i < alternative_storage.size(); i++)
        {
            for (int j = 0; j < alteне работает колёсика steelsereas лфтфrnative_storage[i].size(); j++)
            {
                for (int k = 0; k < alternative_storage[i][j].size(); k++)
                {
                    if (nonterm.find(alternative_storage[i][j][k]) != nonterm.end()) // NONTERM
                        if (gen.find(alternative_storage[i][j][k]) == gen.end()) // OUT OF GEN
                        {
                            inflag = true;
                            break;
                        }
                }

                if (inflag == false)
                    for (int t = 0; t < rule_storage[i].size(); t++)
                    {
                        if (gen.find(rule_storage[i][t]) == gen.end()) // OUT OF GEN
                        {
                            changeflag = true;
                            gen.insert(rule_storage[i][t]);
                        }
                    }
                else
                    inflag = false;
            }
        }

        if (changeflag == false)
            break;
        else
            changeflag = false;
    }

    if (gen.empty() == false)
        for (std::set <std::string>::iterator it = nonterm.begin(); it != nonterm.end(); it++)
            if (gen.find(*it) == gen.end()) // OUT OF GEN
                not_gen.insert(*it);

    cout << ">NOT GENERATING\n";
    for (std::set <std::string>::iterator it = not_gen.begin(); it != not_gen.end(); it++)
        cout << *it << '\n';
    cout << '\n';

    inflag = false;
    changeflag = false;

    del_bad_rules(not_gen);


    /*DELETE NOT REACHABLE NONTERMS*/

    reach.insert(rule_storage[0][0]);

    while (1)
    {
        for (int i = 0; i < rule_storage.size(); i++)
        {
            for (int j = 0; j < rule_storage[i].size(); j++)
            {
                if (nonterm.find(rule_storage[i][j]) != nonterm.end()) // NONTERM
                    if (reach.find(rule_storage[i][j]) != reach.end()) // IN REACH
                    {
                        inflag = true;
                        break;
                    }
            }

            if (inflag == true)
            {
                inflag = false;

                for (int t = 0; t < alternative_storage[i].size(); t++)
                    for (int k = 0; k < alternative_storage[i][t].size(); k++)
                        if (nonterm.find(alternative_storage[i][t][k]) != nonterm.end()) // NONTERM
                            if (reach.find(alternative_storage[i][t][k]) == reach.end()) // OUT OF GEN
                            {
                                changeflag = true;
                                reach.insert(alternative_storage[i][t][k]);
                            }
            }
        }

        if (changeflag == false)
            break;
        else
            changeflag = false;
    }

    for (std::set <std::string>::iterator it = nonterm.begin(); it != nonterm.end(); it++)
        if (reach.find(*it) == reach.end()) // OUT OF GEN
            not_reach.insert(*it);

    cout << ">NOT REACHABLE\n";
    for (std::set <std::string>::iterator it = not_reach.begin(); it != not_reach.end(); it++)
        cout << *it << '\n';
    cout << '\n';

    inflag = false;
    changeflag = false;

    del_bad_rules(not_reach);


    /*DELETE RULES WITH EMPTY PARTS*/

    for (int i = 0; i < alternative_storage.size(); i++)
    {
        for (int j = 0; j < alternative_storage[i].size(); j++)
        {
            for (int k = 0; k < alternative_storage[i][j].size(); k++)
            {
                if (alternative_storage[i][j][k] == eps) // EPS
                {
                    inflag = true;
                    break;
                }
            }

            if (inflag == true)
                for (int t = 0; t < rule_storage[i].size(); t++)
                    empty.insert(rule_storage[i][t]);

            inflag = false;
        }
    }

    inflag = true;
    changeflag = false;

    while (1)
    {
        for (int i = 0; i < alternative_storage.size(); i++)
        {
            for (int j = 0; j < alternative_storage[i].size(); j++)
            {
                for (int k = 0; k < alternative_storage[i][j].size(); k++)
                {
                    if (nonterm.find(alternative_storage[i][j][k]) != nonterm.end()) // NONTERM
                    {
                        if (empty.find(alternative_storage[i][j][k]) == empty.end()) // OUT OF EMPTY
                        {
                            inflag = false;
                            break;
                        }
                    }
                    else
                    {
                        inflag = false;
                        break;
                    }

                }

                if (inflag == true)
                    for (int t = 0; t < rule_storage[i].size(); t++)
                    {
                        if (empty.find(rule_storage[i][t]) == empty.end()) // OUT OF EMPTY
                        {
                            changeflag = true;
                            empty.insert(rule_storage[i][t]);
                        }
                    }
                else
                    inflag = true;
            }
        }

        if (changeflag == false)
            break;
        else
            changeflag = false;
    }

    cout << ">EMPTY" << endl;
    for (std::set <std::string>::iterator it = empty.begin(); it != empty.end(); it++)
        cout << *it << '\n';
    cout << '\n';

    inflag = false;
    changeflag = false;

    //del_eps(empty);

    for (int i = 1; i < alternative_storage.size(); i++)
        for (int j = 0; j < alternative_storage[i].size(); j++)
            for (int k = 0; k < alternative_storage[i][j].size(); k++)
                if (alternative_storage[i][j][k] == eps)
                {
                    alternative_storage[i].erase(alternative_storage[i].begin() + j);
                    j--;
                    break;
                }

    show();

    for (int i = 0; i < alternative_storage.size(); i++)
        for (int j = 0; j < alternative_storage[i].size(); j++)
        {
            new_alternative.clear();
            del_eps(empty, alternative_storage[i][j]);

            /*for (int k = 0; k < new_alternative.size(); k++)
                for (int t = 0; t < new_alternative[k].size(); t++)
                    if (new_alternative[k][t] == ' ')
                        new_alternative[k].erase(new_alternative[k].begin() + t);*/

            for (int k = 0; k < new_alternative.size(); k++)
                {
                    if (find(alternative_storage[i].begin(), alternative_storage[i].end(), new_alternative[k]) == alternative_storage[i].end())
                    {
                        alternative_storage[i].insert(alternative_storage[i].begin(), new_alternative[k]);
                        j++;
                    }
                }
        }

    show();
}

std::set <std::string> Grammar_Storage::make_first (std::string &s, std::set <std::string> &checked)
{
    std::set <std::string> first, tmp_first;

    if (term.find(s) != term.end()) // TERM
    {
        first.insert(s);
        return first;
    }

    for (int i = 0; i < rule_storage.size(); i++)
        for (int j = 0; j < rule_storage[i].size(); j++)
            if (rule_storage[i][j] == s)
                for (int j = 0; j < alternative_storage[i].size(); j++)
                    for (int k = 0; k < alternative_storage[i][j].size(); k++)
                        if (alternative_storage[i][j][k] == eps)
                        {
                            first.insert(eps);
                            break;
                        }

    for (int i = 0; i < rule_storage.size(); i++)
        for (int j = 0; j < rule_storage[i].size(); j++)
            if (rule_storage[i][j] == s)
            {
                for (int j = 0; j < alternative_storage[i].size(); j++)
                    for (int k = 0; k < alternative_storage[i][j].size(); k++)
                        {
                            if (checked.find(alternative_storage[i][j][k]) != checked.end())
                                continue;

                            /*if ((checked.find(alternative_storage[i][j][k]) != checked.end()) && (checked.find(eps) == checked.end()))
                                break;
                            else
                            if (checked.find(alternative_storage[i][j][k]) != checked.end())
                                continue;*/

                            checked.insert(alternative_storage[i][j][k]);
                            tmp_first = make_first(alternative_storage[i][j][k], checked);
                            first = first + tmp_first;

                            if (tmp_first.find(eps) == tmp_first.end())
                                break;
                        }
                break;
            }

    return first;
}

std::set <std::string> Grammar_Storage::make_follow (std::string &s, std::set <std::string> &checked)
{
    std::set <std::string> follow, tmp_follow, tmp_first, tmp_checked;
    std::vector<std::string> tmp_vec;

    if (rule_storage.size() == 0)
        return follow;

    if (s == rule_storage[0][0])
        follow.insert(END);

    for (int i = 0; i < alternative_storage.size(); i++)
        for (int j = 0; j < alternative_storage[i].size(); j++)
            for (int k = 0; k < alternative_storage[i][j].size(); k++)
            {
                if (find(alternative_storage[i][j].begin(), alternative_storage[i][j].end(), s) == alternative_storage[i][j].end())
                    continue;

                if (alternative_storage[i][j][alternative_storage[i][j].size() - 1] == s)
                {
                    if (rule_storage[i][0] != s)
                    {
                        if (checked.find(rule_storage[i][0]) == checked.end())
                        {
                            checked.insert(rule_storage[i][0]);
                            tmp_follow = make_follow(rule_storage[i][0], checked);
                        }

                        follow = follow + tmp_follow;
                    }
                }
                else
                {
                    std::vector<std::string>::iterator it = find(alternative_storage[i][j].begin(), alternative_storage[i][j].end(), s);
                    it++;
                    for (it = it; it != alternative_storage[i][j].end(); it++)
                        tmp_vec.push_back(*it);

                    for (int t = 0; t < tmp_vec.size(); t++)
                    {
                        tmp_checked.clear();
                        tmp_checked.insert(tmp_vec[t]);
                        tmp_first = make_first(tmp_vec[t], tmp_checked);
                        tmp_follow = tmp_follow + tmp_first;

                        if (tmp_first.find(eps) == tmp_first.end())
                            break;
                    }

                    tmp_follow.erase(eps);
                    if (checked.find(rule_storage[i][0]) == checked.end())
                    {
                        checked.insert(rule_storage[i][0]);
                        follow = follow + tmp_follow + make_follow(rule_storage[i][0], checked);
                    }
                    else
                        follow = follow + tmp_follow;
                }
            }

    /*for (std::set <std::string>::iterator it = follow.begin(); it != follow.end(); it++)
        cout << *it << '\n';
    cout << '\n';*/

    return follow;
}

void Grammar_Storage::recursive_descent ()
{
    std::set <std::string> first, follow, tmp;

    for (int i = 0; i < rule_storage.size(); i++)
        for (int j = 0; j < rule_storage[i].size(); j++)
        {
            tmp.clear();
            tmp.insert(rule_storage[i][j]);
            first = make_first(rule_storage[i][j], tmp);

            tmp.clear();
            tmp.insert(rule_storage[i][j]);
            follow = make_follow(rule_storage[i][j], tmp);

            if (check_cross_sets(first, follow) == true)
            {
                cout << ">Recursive descent is not available =(" << endl;
                return;
            }

            /*cout << ">FIRST FOR " << rule_storage[i][j] << endl;
            for (std::set <std::string>::iterator it = first.begin(); it != first.end(); it++)
                cout << *it << '\n';
            cout << '\n';*/ 
        }

    for (int i = 0; i < rule_storage.size(); i++)
        for (int j = 0; j < rule_storage[i].size(); j++)
        {
            tmp.clear();
            tmp.insert(rule_storage[i][j]);
            follow = make_follow(rule_storage[i][j], tmp);

            /*cout << ">FOLLOW FOR " << rule_storage[i][j] << endl;
            for (std::set <std::string>::iterator it = follow.begin(); it != follow.end(); it++)
                cout << *it << '\n';
            cout << '\n';*/
        }

    /*if (check_cross_sets(first, follow) == false)
        cout << ">Recursive descent is available =)" << endl;
    else*/
        cout << ">Recursive descent is not available =(" << endl;
}


Reader::Reader (const char *s)
{
    file_name.open(s, ios_base::in);
    if (!file_name.is_open())
        throw ">Can not find file with such name.";

    col_num = 1;
    str_num = 1;
}

Reader::~Reader ()
{
    file_name.close();
}

int Reader::getletter()
{
    col_num++;

    letter = file_name.get();

    if (letter == '\n')
    {
        str_num++;
        col_num = 1;
    }

    /*while ((letter == ' ') || (letter == '\t'))
        letter = file_name.get();*/

    return letter;
}

void Reader::clear_comments ()
{
    if ((letter != '{') && (letter != '/'))
        return;

    if (letter == '{')
    {
        while (getletter() != '}');
    }
    else
    if (letter == '/')
    {
        getletter();
        while (1)
            if (getletter() == '*')
                if (getletter() == '/')
                    break;
    }

    if (getletter() == '\n')
        getletter();
    while ((letter == ' ') || (letter == '\t'))
            getletter();
    clear_comments();

    return;
}

Grammar_Storage Reader::get_grammar ()
{
    Grammar_Storage storage;

    std::vector<string> tmp_vec;
    std::vector< std::vector<string> > push_vec;
    std::string tmp_str;

    letter = 0;

    file_name >> tmp_str;
    col_num += tmp_str.size() + 1;

    while (tmp_str.compare("%%") != 0)
    {
        if (tmp_str.compare(1, 5, "token") == 0)
        {
            file_name >> tmp_str;
            col_num += tmp_str.size() + 1;

            while (tmp_str[tmp_str.size() - 1] != ';')
            {
                storage.push_term(tmp_str);
                file_name >> tmp_str;
                col_num += tmp_str.size() + 1;
            }

            tmp_str = tmp_str.substr(0, tmp_str.size() - 1);
            storage.push_term(tmp_str);
            str_num++;
            col_num = 0;
        }

        file_name >> tmp_str;
        if (file_name.eof() == true)
            throw ">There is no gramma in file.";
        //cout << tmp_str;
        col_num += tmp_str.size() + 1;
        if ((tmp_str[tmp_str.size() - 1] == ';') || (tmp_str.empty() == true))
        {
            str_num++;
            col_num = 0;
        }
    }
    tmp_str.clear();

    while (getletter() == '\n');

    while (letter != EOF)
    {
        while (letter != ':')
        {
            if (letter == ' ')
            {
                tmp_vec.push_back(tmp_str);
                tmp_str.clear();
            }
            else
                tmp_str += letter;

            if (getletter() == '\n') /*!!!!!!!!!!!!!!*/
                getletter();

            if (letter == '/')
            {
                tmp_str += letter;
                if (getletter() == '*')
                {
                    tmp_str.erase(tmp_str.end() - 1);
                    letter = '/';
                    clear_comments();
                    continue;
                }
            }
            clear_comments();
        }

        if (tmp_str.empty() == false)
            tmp_vec.push_back(tmp_str);
        storage.push_rule(tmp_vec);
        tmp_str.clear();
        tmp_vec.clear();

        getletter(); // \n || ' '
        if ((letter != '\n') && (letter != ' '))
        {
            cout << col_num << ' ' << str_num << endl;
            throw ">Unexpectable symbol, expected space or '\\n'";
        }
        getletter();
        while ((letter == ' ') || (letter == '\t'))
            getletter();
        clear_comments();

        if (letter == '|')
        {
            tmp_str += eps;
            tmp_vec.push_back(tmp_str);
            push_vec.push_back(tmp_vec);
            tmp_str.clear();
            tmp_vec.clear();
            getletter(); // ' '
            if (letter != ' ')
            {
                cout << col_num << ' ' << str_num << endl;
                throw ">Unexpectable symbol, expected space";
            }
            getletter();
        }

        while (letter != ';')
        {
            if ((letter == ' ') || (letter == '\n'))
            {
                if (tmp_str.empty() == false)
                    tmp_vec.push_back(tmp_str);
                tmp_str.clear();

                if (letter == ' ')
                {
                    while ((getletter() == ' ') || (letter == '\t'));
                    clear_comments();
                    continue;
                }
            }
            else
            if (letter == '|')
            {
                if (tmp_str.empty() == false)
                    tmp_vec.push_back(tmp_str);

                push_vec.push_back(tmp_vec);
                tmp_str.clear();
                tmp_vec.clear();
                getletter(); // ' '
                if (letter != ' ')
                {
                    cout << col_num << ' ' << str_num << endl;
                    throw ">Unexpectable symbol, expected space";
                }
            }
            else
                tmp_str += letter;

            if (letter == '\n')
                while ((getletter() == ' ') || (letter == '\t'));
            else
                getletter();

            if (letter == '/')
            {
                tmp_str += letter;
                if (getletter() == '*')
                {
                    tmp_str.erase(tmp_str.end() - 1);
                    letter = '/';
                    clear_comments();
                    continue;
                }
            }
            clear_comments();
        }

        if (tmp_str.empty() == false)
            tmp_vec.push_back(tmp_str);
        push_vec.push_back(tmp_vec);
        storage.push_alternative(push_vec);
        tmp_str.clear();
        tmp_vec.clear();
        push_vec.clear();

        /*while (getletter() == '\n');*/
        getletter();
        while ((letter == ' ') || (letter == '\t') || (letter == '\n'))
            getletter();

        if (letter == '%')
        {
            if (getletter() == '%')
                break;
        }
    }

    storage.make_term();
    storage.make_nonterm();

    return storage;
}