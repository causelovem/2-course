enum lexeme_type
{
    LEXEME_WORD,
    LEXEME_MLIST,
    LEXEME_LIST,
    LEXEME_SPACE,
    LEXEME_HEADER,
    LEXEME_BLANK,
    LEXEME_NULL,
    LEXEME_CODE
};

class Token
{
    private:
        Mystring str;
        lexeme_type type;

    public:
        Token(lexeme_type t, const char *s): type(t), str(s) {};
        
        void show()const
        {
            std::cout << str.ptr() << std::endl;

            return;
        }

        Mystring getstr()const
        {
            return str;
        }

        lexeme_type gettype()const
        {
            return type;
        }    
};

class Token_Word: public Token
{
    public:
        Token_Word(const char *w): Token(LEXEME_WORD, w) {};    
};

class Token_Mlist: public Token
{
    public:
        Token_Mlist(): Token(LEXEME_MLIST, "*") {};    
};

class Token_List: public Token
{
    public:
        Token_List(const char *s): Token(LEXEME_LIST, s) {};    
};

class Token_Space: public Token
{
    public:
        Token_Space(const char *s): Token(LEXEME_SPACE, s) {};  
};

class Token_Header: public Token
{
    public:
        Token_Header(const char *s): Token(LEXEME_HEADER, s) {};
};

class Token_Blank: public Token
{
    public:
        Token_Blank(): Token(LEXEME_BLANK, "") {};
};

class Token_Null: public Token
{
    public:
        Token_Null(): Token(LEXEME_NULL, "") {}; 
};

class Token_Code: public Token
{
    public:
        Token_Code(const char *w): Token(LEXEME_CODE, w) {};
};

class Reader
{
    private:
        enum state
            {S, SPACES, GRIDS, MLIST, LIST, RAW, CODE, HEADER, LISTELEM};

        state ST;
        char buffer[1024];
        int pointer;
        FILE *name;
        int letter;

        int getletter()
        {
            return (letter = fgetc(name));
        }

        void push()
        {
            buffer[pointer++] = letter;
            buffer[pointer] = '\0';

            return;
        }

        void push(int ch)
        {
            buffer[pointer++] = ch;
            buffer[pointer] = '\0';

            return;
        }

        void push_space()
        {
            if ((pointer == 0) || (buffer[pointer - 1] != ' '))
                push(' ');

            return;
        }

        void clear()
        {
            buffer[(pointer = 0)] = '\0';
            
            return;
        }

    public:
        int words_count;

        Reader(const char *s)
        {
            if ((name = fopen(s, "r")) == NULL)
                throw ">Can not find file with such name.";

            ST = S;
            words_count = 0;
            clear();
            getletter();
        };

        ~Reader()
        {
            fclose(name);

            return;
        };

        Token *get_lexeme();   
};

Token *Reader::get_lexeme()
{
    int spaces = 0;
    
    clear();

    while(1)
    {
        switch(ST)
        {
            case S:
                if (letter == ' ')
                {
                    ST = SPACES;
                }
                else
                if (letter == '#')
                {
                    ST = GRIDS;
                }
                else
                if (letter == '*')
                {
                    push();
                    ST = MLIST;
                    getletter();
                }
                else
                if (isdigit(letter) != 0)
                {
                    push();
                    ST = LIST;
                    getletter();
                }
                else
                if (letter == '\n')
                {
                    while (getletter() == '\n');
                    return new Token_Blank();
                }
                else
                if (letter == EOF)
                {
                    return new Token_Null();
                }
                else
                {
                    ST = RAW;
                }
            break;

            case SPACES:
                while (letter == ' ')
                {
                    push();
                    spaces++;
                    getletter();
                }

                if ((letter == '\n') || (letter == EOF))
                {
                    ST = S;
                    return new Token_Blank();
                }
                
                if (spaces == 4)
                {
                    ST = CODE;
                    return new Token_Space(buffer);
                    break;
                }

                ST = S;
                return new Token_Space(buffer);
            break;

            case GRIDS:
                while (letter == '#')
                {
                    push();
                    getletter();
                }

                if (letter == ' ')
                {
                    while (getletter() == ' ');

                    if ((letter == '\n') || (letter == EOF))
                        ST = RAW;
                    else
                    {
                        ST = HEADER;
                        return new Token_Header(buffer);
                    }
                }
                else
                    ST = RAW;
            break;

            case MLIST:
                if (letter == ' ')
                {
                    while (getletter() == ' ');

                    if ((letter == '\n') || (letter == EOF))
                    {
                        ST = RAW;
                    }
                    else
                    {
                        ST = LISTELEM;
                        return new Token_Mlist();
                    }

                }
                else ST = RAW;
            break;

            case LIST:
                while (isdigit(letter) != 0)
                {
                    push();
                    getletter();
                }

                if (letter == '.')
                {
                    push();
                    getletter();

                    if (letter == ' ')
                    {
                        while (getletter() == ' ');

                        if ((letter == '\n') || (letter == EOF))
                            ST = RAW;
                        else
                        {
                            ST = LISTELEM;
                            return new Token_List(buffer);
                        }
                    }
                    else
                        ST = RAW;
                }
                else
                    ST = RAW;
            break;

            case RAW:
                while ((letter != '\n') && (letter != EOF))
                {
                    if (letter == ' ')
                    {
                        push_space();
                        getletter();
                    }
                    else
                    if (letter == '\\')
                    {
                        push();
                        getletter();

                        if ((letter == '\\') || (letter == '#') || letter == '*')
                        {
                            buffer[strlen(buffer) - 1] = letter;
                            getletter();
                            continue;
                        }
                    }
                    else
                    {
                        push();
                        getletter();
                    }
                }

                if (letter == '\n')
                    getletter();

                ST = S;
                return new Token_Word(buffer);
            break;
                
            case CODE:
                while ((letter != '\n') && (letter != EOF))
                {
                    if (letter == ' ')
                    {
                        push_space();
                        getletter();
                    }
                    else
                    if (letter == '\\')
                    {
                        push();
                        getletter();
                            
                        if ((letter == '\\') || (letter == '#') || letter == '*')
                        {
                            buffer[strlen(buffer) - 1] = letter;
                            getletter();
                            continue;
                        }
                    }
                    else
                    {
                        push();
                        getletter();
                    }
                }
                
                if (letter == '\n')
                    getletter();
                
                if ((letter == '\n') || (letter == EOF))
                {
                    ST = S;
                    return new Token_Code(buffer);
                }
                else
                    ST = CODE;
                
                spaces = 0;
            break;

            case HEADER:
                while ((letter != '\n') && (letter != EOF))
                {
                    if (letter == ' ')
                    {
                        words_count++;
                        push();
                        while (getletter() == ' ');
                    }
                    else
                    if (letter == '\\')
                    {
                        push();
                        getletter();

                        if ((letter == '\\') || (letter == '#') || (letter == '*'))
                        {
                            buffer[strlen(buffer) - 1] = letter;
                            getletter();
                            continue;
                        }
                    }
                    else
                    {
                        push();
                        getletter();
                    }
                }

                if (letter == '\n')
                    getletter();

                ST = S;
                return new Token_Word(buffer);
            break;

            case LISTELEM:
                while ((letter != '\n') && (letter != EOF))
                {
                    if (letter == ' ')
                    {
                        push();
                        while (getletter() == ' ');
                    }
                    else
                    if (letter == '\\')
                    {
                        push();
                        getletter();

                        if ((letter == '\\') || (letter == '#') || (letter == '*'))
                        {
                            buffer[strlen(buffer) - 1] = letter;
                            getletter();
                            continue;
                        }
                    }
                    else
                    {
                        push();
                        getletter();
                    }
                }

                if (letter == '\n')
                    getletter();

                ST = S;
                return new Token_Word(buffer);
            break;
        }
    }
}