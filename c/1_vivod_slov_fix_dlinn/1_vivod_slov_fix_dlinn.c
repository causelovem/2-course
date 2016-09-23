#include <stdio.h>
#define MAXLEN 256
#define FILENAMELEN 30

int filefilter(FILE*f,int maxwordlen);

int main(int argc,char **argv)
{
	FILE *f;

	char filename[FILENAMELEN];
	int maxwordlen;


	while (1)
	{
		printf(">Введите имя файла.\n");
		scanf("%s",filename);
		f=fopen(filename,"r");

		if (f!=NULL)
			break;

		printf("===Неверное имя файла.===\n");
	}


	while (1)
	{	
		printf(">Введите максимальное количество букв (1..256).\n");
		scanf("%i",&maxwordlen);

		if ((maxwordlen>=1)&&(maxwordlen<=256))
			break;

		printf("===Неверная длина слова.===\n");
	}

	filefilter(f,maxwordlen);
	fclose(f);
	return 0;
}

int filefilter(FILE*f,int maxwordlen)
{
	int i,curmas=0;
	char letter=' ';
	char wordbuf [MAXLEN+1];

	while (letter!=EOF)
	{
		letter=getc(f);

		if ((letter==EOF)||(letter=='\n')||(letter==' '))
		{
			wordbuf[curmas]='\0';

			if (curmas<=maxwordlen)
					printf("%s",wordbuf);

			curmas=0;

			if (letter==' ')
				putchar(' ');			
			else if (letter=='\n')
					putchar('\n');
		}
		else
			wordbuf[curmas++]=letter;
	}
	return 0;
}