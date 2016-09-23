#include <cstring>

class Mystring
{
	private:
		char *my_str;

	public:
		Mystring()
		{
			my_str = (char *) calloc (1, sizeof(char));

			return;
		}

		Mystring(const char *s)
		{
			if (s != NULL)
			{
				my_str = (char * ) malloc ((strlen(s) + 1) * sizeof(char));
				strcpy(my_str, s);
			}

			return;
		}

		Mystring(const Mystring &s)
		{
			my_str = (char *) malloc ((strlen(s.my_str) + 1) * sizeof(char));
			strcpy(my_str, s.my_str);

			return;
		}

		Mystring operator = (const Mystring &s)
		{
			if (my_str != NULL)
				free(my_str);
			my_str = (char *) malloc ((strlen(s.my_str) + 1) * sizeof(char));
			strcpy(my_str, s.my_str);

			return *this;
		}

		int operator == (const char *s)
		{
			if (strcmp(my_str, s) == 0)
				return 1;
			else
				return 0;
		}

		int operator != (const char *s)
		{
			if (strcmp(my_str, s) == 0)
				return 0;
			else
				return 1;
		}

		Mystring operator += (const char *s)
		{
			my_str = (char *) realloc (my_str, (strlen(my_str) + strlen(s) + 1) * sizeof(char));
			strcat(my_str,s);

			return *this;
		}

		Mystring operator += (const Mystring &s)
		{
			my_str = (char *) realloc (my_str, (strlen(my_str) + strlen(s.my_str) + 1) * sizeof(char));
			strcat(my_str,s.my_str);

			return *this;
		}

		int len()const
		{
			return ((int) strlen(my_str));
		}

		char *ptr()const
		{
			return my_str;
		}

		~Mystring()
		{
			if (my_str != NULL)
				free(my_str);
			my_str = NULL;
		}	
};