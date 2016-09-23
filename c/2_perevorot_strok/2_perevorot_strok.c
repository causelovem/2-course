#include <stdio.h>
#define MAXLEN 257

int swap(char *buf,int len);

int main()
{
	char stringbuf[MAXLEN];
	int letter=' ';
	int curmas=0;

	while (letter!=EOF)
	{
		letter=getchar();

		if ((letter==EOF)||(letter=='\n'))
		{
			swap(stringbuf,curmas-1);
			curmas=0;
			if (letter=='\n')
				putchar('\n');
		}
		else
			stringbuf[curmas++]=letter;			
	}

	return 0;
}

int swap(char *buf,int len)
{
	int i;
	for (i=len;i>-1;i=i-1)
	{
		putchar((int)(buf[i]));
	}
	return 0;
}











