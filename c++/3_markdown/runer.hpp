#define FIRST_UNICODE_BYTE1 (char)0xD0
#define FIRST_UNICODE_BYTE2 (char)0xD1

int width = 40, tab = 2, red = 0;

char mlist_marker = '*';

enum text_type
{
    TEXT_RAW,
    TEXT_MLIST,
    TEXT_LIST,
    TEXT_HEADER, 
    TEXT_NULL,
    TEXT_CODE
};

int count_digits(int number){ return ( number /= 10 ) ? 1 + count_digits(number) : 1; }

class Abstract_text
{
    private:
        Mystring text;
        text_type type;

    public:
        Abstract_text(text_type t = TEXT_NULL): type(t) {}

        Abstract_text(const Mystring txt, text_type t): text(txt), type(t) {}

        Mystring gettext()const
        { 
            return text;
        };

        text_type gettype()const
        {
            return type;
        };

        virtual unsigned long int count_symbols()const = 0;
        virtual unsigned long int count_words()const = 0;
        virtual void print()const = 0;

        virtual ~Abstract_text() {};
};

class Text_Storage
{
    private:
        Abstract_text **storage;
        int _size;

    public:
        Text_Storage(): storage(NULL), _size(0) {};

        ~Text_Storage() 
        { 
            for (int i = 0; i < _size; delete storage[i++]); 
            free(storage);
        };

        void push(Abstract_text *T)
        {
            storage = (Abstract_text **) realloc(storage, ++_size * sizeof(Abstract_text *));
            storage[_size - 1] = T;

            return;
        };

        Abstract_text *top()const
        {
            return (_size ? storage[_size - 1] : NULL);
        };

        Abstract_text * operator [] (int pos)const
        {
            return storage[pos];
        };

        int size()const
        {
            return _size;
        };
};

class Raw_text: public Abstract_text
{
    private:
        int indent;

    public:
        Raw_text(const Mystring txt, int ind): Abstract_text(txt, TEXT_RAW), indent(ind) {};

        unsigned long int count_symbols()const
        {
            int symbols_count = 0, i = 0, tmp_width = 0;

            Mystring str = gettext();
            for (char *word = strtok(str.ptr(), " "); word; word = strtok(NULL, " "))
            {
                i = 0;
                tmp_width = 0;
                while (word[i] != '\0')
                {
                    if ((word[i] == FIRST_UNICODE_BYTE1) || (word[i] == FIRST_UNICODE_BYTE2))
                        tmp_width++;
                    i++;
                }
                
                symbols_count += strlen(word) - tmp_width;
            }

            return symbols_count;
        }

        unsigned long int count_words()const
        {
            int words = 0;

            Mystring str = gettext();
            for (char *word = strtok(str.ptr(), " "); word; word = strtok(NULL, " ")) 
                ++words;

            return words;
        }

        void set_indent(int ind)
        {
            indent = ind;
        }

        virtual void print() const
        {
            int cur_width = width - indent;
            int tmp_width = 0, i = 0;
            bool newline = indent ? false : true;

            Mystring str = gettext();
            char *word = strtok(str.ptr(), " ");
            while (word)
            {
                i = 0;
                tmp_width = 0;
                while (word[i] != '\0')
                {
                    if ((word[i] == FIRST_UNICODE_BYTE1) || (word[i] == FIRST_UNICODE_BYTE2))
                        tmp_width++;
                    i++;
                }
                
                if (strlen(word) - tmp_width > width - indent)
                    throw ">Too long word.";

                if (newline)
                {
                    newline = false;
                    for (int i = 0; i++ < indent; std::cout << ' ');
                    std::cout << word;
                    cur_width -= strlen(word) - tmp_width;
                }
                else
                {
                    if (strlen(word) - tmp_width >= cur_width)
                    {
                        std::cout << std::endl;
                        newline = true;
                        cur_width = width - indent;
                        continue;
                    }
                    else
                    {
                        std::cout << ' ' << word;
                        cur_width -= strlen(word) - tmp_width + 1;
                    }
                }
                word = strtok(NULL, " ");
            }
            std::cout << std::endl;
        }
};

class Header: public Abstract_text
{
    private:
        int level; 

    public:
        Header(int l, const Mystring txt): Abstract_text(txt, TEXT_HEADER), level(l) {};

        unsigned long int count_symbols()const
        {
            int symbols = 0, i = 0, tmp_width = 0;

            Mystring str = gettext();
            for (char *word = strtok(str.ptr(), " "); word; word = strtok(NULL, " "))
            {
                i = 0;
                tmp_width = 0;
                while (word[i] != '\0')
                {
                    if ((word[i] == FIRST_UNICODE_BYTE1) || (word[i] == FIRST_UNICODE_BYTE2))
                        tmp_width++;
                    i++;
                }
                
                symbols += strlen(word) - tmp_width;
            }

            return symbols;
        }

        unsigned long int count_words()const
        {
            int words = 0;

            Mystring str = gettext();
            for (char *word = strtok(str.ptr(), " "); word; word = strtok(NULL, " ")) 
                ++words;

            return words;
        }

        virtual void print()const
        {
            int i = 0, tmp_width = 0;
            Mystring str = gettext();
            char *header = gettext().ptr();
            //header = (char *) realloc (header, )
            //char *header = strtok(str.ptr(), " ");

            
            i = 0;
            tmp_width = 0;
            //while (header[i] != '\0')
            while (i != str.len())
            {
                if ((gettext().ptr()[i] == FIRST_UNICODE_BYTE1) || (gettext().ptr()[i] == FIRST_UNICODE_BYTE2))
                    tmp_width++;
                i++;
            }
            
            if (gettext().len() - tmp_width + 2*level > width)
                throw ">Can't fit header in a line.";

            for (int i = 0; i++ < width; std::cout << '#');
            std::cout << std::endl;

            for (int i = 0; i++ < level; std::cout << '#');

            int spaces = (width - 2*level - gettext().len() + tmp_width)/2;
            for (int i = 0; i++ < spaces; std::cout << ' ');

            std::cout << gettext().ptr();

            spaces += (width - 2*level - gettext().len() + tmp_width)%2;
            for (int i = 0; i++ < spaces; std::cout << ' ');

            for (int i = 0; i++ < level; std::cout << '#');
            std::cout << std::endl;

            for (int i = 0; i++ < width; std::cout << '#');
            std::cout << std::endl;
        }
};

class Code: public Abstract_text
{
    public:
        Code(const Mystring txt): Abstract_text(txt, TEXT_CODE) {};
    
        unsigned long int count_symbols()const
        {
            int symbols = 0, i = 0, tmp_width = 0;
        
            Mystring str = gettext();
            for (char *word = strtok(str.ptr(), " "); word; word = strtok(NULL, " "))
            {
                i = 0;
                tmp_width = 0;
                while (word[i] != '\0')
                {
                    if ((word[i] == FIRST_UNICODE_BYTE1) || (word[i] == FIRST_UNICODE_BYTE2))
                        tmp_width++;
                    i++;
                }
            
                symbols += strlen(word) - tmp_width;
            }
        
            return symbols;
        }
    
        unsigned long int count_words()const
        {
            int words = 0;
        
            Mystring str = gettext();
            for (char *word = strtok(str.ptr(), " "); word; word = strtok(NULL, " "))
                ++words;
        
            return words;
        }
    
        virtual void print()const
        {
            //int cur_width = 0, tmp_width = 0, i = 0;
            int i = 0, cur_width = width;
            bool newcode = true;
            
            Mystring str = gettext();
            char *word = strtok(str.ptr(), ";");
            //std::cout << "@   ";
            while (word)
            {
                /*i = 0;
                tmp_width = 0;
                while (word[i] != '\0')
                {
                    if ((word[i] == FIRST_UNICODE_BYTE1) || (word[i] == FIRST_UNICODE_BYTE2))
                        tmp_width++;
                    i++;
                }*/
                
                i = 0;
                cur_width = width;
                std::cout << "@   ";
                if (newcode)
                {
                    newcode = false;
                    //std::cout << "@   ";
                }
                else
                {
                    i = 1;
                    cur_width++;
                    //std::cout << "@   ";
                }
                
                while ((i != strlen(word)) && (i < cur_width - 4))
                    std::cout << word[i++];
                
                if (i == strlen(word))
                    std::cout << ";\n";
                else
                    std::cout << "\n";
                
                word = strtok(NULL, ";");
            }
            //std::cout << std::endl;
        }
};

class List: public Abstract_text
{
    private:
        Text_Storage elems;
        int level;

    public:
        List(text_type t, int l): Abstract_text(t), level(l) {}

        Text_Storage &getelem()
        {
            return elems;
        }

        unsigned long int count_symbols()const
        {
            int symbols = 0;

            for (int i = 0; i < elems.size(); ++i)
                symbols += elems[i]->count_symbols();

            return symbols;
        }

        unsigned long int count_words()const
        {
            int words = 0;

            for (int i = 0; i < elems.size(); ++i)
                words += elems[i]->count_words();

            return words;
        }

        virtual void print() const
        {
            for (int i = 0; i < elems.size(); ++i)
            {
                if ((elems[i]->gettype() == TEXT_LIST) || (elems[i]->gettype() == TEXT_MLIST))
                {
                    elems[i]->print();
                }
                else if (gettype() == TEXT_LIST)
                {
                    for (int j = 0; j++ < level*tab; std::cout << ' ');
                    std::cout << i+1 << '.';
                    dynamic_cast<Raw_text *>(elems[i])->set_indent(level*tab + count_digits(i+1) + 2);
                    elems[i]->print();
                }
                else
                {
                    for (int j = 0; j++ < level*tab; std::cout << ' ');
                    std::cout << mlist_marker;
                    elems[i]->print();
                }
            }
        }
};

class Runer
{
    Reader &S;
    Token *cur_lex;
    Text_Storage &storage;

public:
    Runer(Reader &s, Text_Storage &ts): S(s), storage(ts) { cur_lex = S.get_lexeme(); };
    
    ~Runer()
    {
        delete cur_lex;
    };

    int scan_list(Abstract_text *L, int s, int level);
    void run();

    Token *getlex()
    {
        delete cur_lex;

        return cur_lex = S.get_lexeme();
    };
};

int Runer::scan_list(Abstract_text *L, int s, int level)
{
    int spaces = s;
    Mystring tmp;

    while (1)
        switch (cur_lex->gettype())
        {
            case LEXEME_BLANK:
                getlex();
                
            case LEXEME_NULL:
                dynamic_cast<List *>(L)->getelem().push(new Raw_text(tmp, level*tab+2));
                return -1;
            break;
            
            case LEXEME_SPACE:
                spaces = cur_lex->getstr().len();
                getlex();
            break;

            case LEXEME_MLIST:
            case LEXEME_LIST:
                if (tmp.len())
                {
                    dynamic_cast<List *>(L)->getelem().push(new Raw_text(tmp, level*tab+2));
                    tmp = "";
                }

                if (spaces == s) 
                {
                    getlex();   
                }
                else
                if (spaces < s) 
                {
                    return spaces; 
                }
                else
                if (spaces > s) 
                { 
                    if (cur_lex->gettype() == LEXEME_LIST)
                        dynamic_cast<List *>(L)->getelem().push(new List(TEXT_LIST, level + 1));
                    else 
                        dynamic_cast<List *>(L)->getelem().push(new List(TEXT_MLIST, level + 1));

                    spaces = scan_list(dynamic_cast<List *>(L)->getelem().top(), spaces, level + 1);
                    break;
                }

                spaces = 0;
            break;

            case LEXEME_HEADER:
                tmp += cur_lex->getstr() += " ";
                getlex();

            case LEXEME_WORD:
                tmp += cur_lex->getstr() += " ";
                getlex();
                spaces = 0;
            break;
                
            case LEXEME_CODE:
                tmp += cur_lex->getstr() += " ";
                getlex();
                spaces = 0;
            break;
        }
}

void Runer::run()
{
    int spaces = 0;
    Mystring tmp = "";

    while (1)
        switch (cur_lex->gettype())
        {
            case LEXEME_BLANK:
                if (tmp.len()) storage.push(new Raw_text(tmp, 0));

                spaces = 0;
                tmp = "";
                getlex();
            break;

            case LEXEME_WORD:
                spaces = 0;
                tmp += cur_lex->getstr() += " ";
                getlex();
            break;

            case LEXEME_HEADER:
                if (tmp.len())
                    tmp += cur_lex->getstr() += " ";
                else
                    storage.push(new Header(cur_lex->getstr().len(), getlex()->getstr()));

                getlex();
            break;

            case LEXEME_SPACE:
                spaces = cur_lex->getstr().len();
                getlex();
            break;
                
            case LEXEME_CODE:
                //spaces = 0;
                storage.push(new Code(cur_lex->getstr()));
                getlex();
            break;

            case LEXEME_LIST:
                if (tmp.len())
                    tmp += cur_lex->getstr() += " ";
                else
                {
                    getlex();
                    storage.push(new List(TEXT_LIST, 1));
                    scan_list(storage.top(), spaces, 1);
                }
            break;

            case LEXEME_MLIST:
                if (tmp.len())
                    tmp += cur_lex->getstr() += " ";
                else
                {
                    getlex();
                    storage.push(new List(TEXT_MLIST, 1));
                    scan_list(storage.top(), spaces, 1);
                }
            break;

            default:  
                if (tmp.len())
                    storage.push(new Raw_text(tmp, 0));
                return;
        }
}