#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <climits>

#include "class_Exception.hpp"

Exception::Exception()
{
	err_message = NULL;

	return;
}

Exception::Exception(const char *arg)
{
	if (arg == NULL)
	{
		err_message = NULL;
		return;
	}

	err_message = (char *) malloc ((strlen(arg) + 1) * sizeof(char));
	for (int i = 0; i < strlen(arg); i++)
		err_message[i] = arg[i];

	err_message[strlen(arg)] = '\0';

	return;
}

Exception::Exception(const Exception &arg)
{
	if (arg.err_message == NULL)
	{
		err_message = NULL;
		return;
	}

	err_message = (char *) malloc ((strlen(arg.err_message) + 1) * sizeof(char));
	for (int i = 0; i < strlen(arg.err_message); i++)
		err_message[i] = arg.err_message[i];

	err_message[strlen(arg.err_message)] = '\0';

	return;
}

Exception::~Exception()
{
	if (err_message != NULL)
		free(err_message);

	err_message = NULL;

	return;
}

void Exception::show(FILE *f)const
{
	fprintf(f, "Exception: %s\n", err_message);

	return;
}
