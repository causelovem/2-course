#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

using namespace std;

class String
{
	private:
		char *body;
		size_t str_len;

	public:
		String operator + (const char *arg);
		String operator + (const String &str1);
		friend String operator + (const char *arg1, const String &arg2);
		String operator = (const String &str);
		String operator = (const char *str);
		void showstr ();

		String ();
		String (const char *str);
		String (const String &arg);
		String (FILE *f);
		~String ();
};

class Str_err
{
private:
	int err_code;
	int err_file;

public:
	Str_err (const int err_value): err_code(err_value) {};
	Str_err (): err_file(1) {};
	void print_error()
	{
		if (err_file == 0)
		{
			printf("Can not allocate %i bytes\n", err_code);
			exit(-1);
		}
		else
		{
			printf("Can not open this file\n");
			exit(-1);
		}

	}
};

String::String ()
{
	str_len = 0;
	body = (char *) calloc(1, sizeof(char));

	return;
}

String::String (const char *str)
{
	if (str == NULL)
		String ();
	else
	{
		str_len = strlen(str);
		body = (char *) malloc ((str_len + 1) * sizeof(char));
		if (body == NULL)
		{
			Str_err error(str_len + 1);
			throw error;
		}

		for(int i = 0;i < str_len; i++)
			body[i] = str[i];

	}

	return;
}

String::String (const String &arg)
{
	if (arg.body == NULL)
		String ();
	else
	{
		str_len = arg.str_len;
		body = (char *) malloc ((str_len + 1) * sizeof(char));
		if (body == NULL)
		{
			Str_err error(str_len + 1);
			throw error;
		}

		for(int i = 0;i < str_len; i++)
			body[i] = arg.body[i];

	}

	return;
}

String::String (FILE *f)
{
	int letter = 0;

	if ((letter = getc(f)) == EOF)
	{
		String ();
	}
	else
	{
		str_len = 1;
		body = (char *) malloc (str_len * sizeof(char));
		if (body == NULL)
		{
			Str_err error(str_len);
			throw error;
		}
		body[str_len - 1] = letter;

		letter = getc(f);

		while ((letter != EOF) && (letter != '\n'))
		{
			str_len++;
			body = (char *) realloc (body, str_len * sizeof(char));
			if (body == NULL)
			{
				Str_err error(str_len);
				throw error;
			}
			body[str_len - 1] = letter;

			letter = getc(f);
		}
			str_len++;
			body = (char *) realloc (body, str_len * sizeof(char));
			if (body == NULL)
			{
				Str_err error(str_len);
				throw error;
			}
			body[str_len - 1] = '\n';
	}

	return;
}

String String::operator = (const String &str)
{
	if (this == &str)
		return *this;

	if (body != NULL)
		free(body);

	str_len = str.str_len;
	body = (char *) malloc (str_len * sizeof(char));
	if (body == NULL)
	{
		Str_err error(str_len);
		throw error;
	}

	for(int i = 0; i < str_len; i++)
		body[i] = str.body[i];

	return *this;
}

String String::operator = (const char *str)
{
	if (body == str)
		return *this;

	if (body != NULL)
		free(body);

	if (str == NULL)
	{
		body = (char *) calloc (1, sizeof(char));
		str_len = 0;
		return *this;
	}

	str_len = strlen(str);
	body = (char *) malloc ((str_len + 1) * sizeof(char));
	if (body == NULL)
	{
		Str_err error(str_len + 1);
		throw error;
	}

	for(int i = 0; i < str_len; i++)
		body[i] = str[i];

	return *this;
}

String String::operator + (const char *arg)
{
	/*if (arg == NULL)
	{
		String str(*this);
		return str;
	}*/

	String str(*this);

	str.str_len = strlen(arg) + this->str_len;

	str.body = (char *) realloc (str.body, (str.str_len + 1) * sizeof(char));
	if (str.body == NULL)
	{
		Str_err error(str.str_len + 1);
		throw error;
	}

	for(int i = 0; i < strlen(arg); i++)
		str.body[i + this->str_len] = arg[i];

	return str;
}

String String::operator + (const String &arg)
{
	String str(*this);

	str.str_len = arg.str_len + this->str_len;
	str.body = (char *) realloc (str.body, (str.str_len + 1) * sizeof(char));
	if (str.body == NULL)
		{
			Str_err error(str.str_len + 1);
			throw error;
		}
	
	for(int i = 0; i < arg.str_len; i++)
		str.body[i + this->str_len] = arg.body[i];

	return str;
}

String operator + (const char *arg1, const String &arg2)
{
	/*if (arg1 == NULL)
	{
		String str(arg2);
		return str;
	}*/

	String str(arg1);

	str.str_len = strlen(arg1) + arg2.str_len;

	str.body = (char *) realloc (str.body, (str.str_len + 1) * sizeof(char));
	if (str.body == NULL)
		{
			Str_err error(str.str_len + 1);
			throw error;
		}
	
	for(int i = 0; i < arg2.str_len; i++)
		str.body[i + strlen(arg1)] = arg2.body[i];;

	return str;
}

void String::showstr ()
{
	if (body != NULL)
		for (int i = 0; i < str_len; i++)
			cout<<body[i];
	cout<<endl;

	return;
}

String::~String ()
{
	if (body != NULL)
		free(body);

	body = NULL;
	str_len = 0;

	return;
}

int main ()
{
	FILE *fi;
	const char *sss = NULL;
	Str_err error;

	try
	{
		if ((fi = fopen("file", "r")) == NULL)
			throw error;

		String str1;
		String str2 ("Hello");
		String str3 (str2);
		String str4(fi);

		cout << "Test 1"<<endl;
		str1.showstr();
		str2.showstr();
		str3.showstr();
		str4.showstr();
		cout<<endl;

		str1 = ", world!";
		str2 = str2 + ", world!";
		str3 = str3 + str2;
		str4 = sss;

		cout << "Test 2"<<endl;
		str1.showstr();
		str2.showstr();
		str3.showstr();
		str4.showstr();
		cout<<endl;

		str2 = str3;
		str3 = "Bye" + str1;

		cout << "Test 3"<<endl;
		str2.showstr();
		str3.showstr();
		cout<<endl;

		fclose(fi);
	}
	catch (Str_err error)
	{
		error.print_error();
	}

	return 0;
}