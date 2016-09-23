#include <stdio.h>
#define MAXLEN 256
#define FILENAMELEN 30

int filefilter(FILE*f,int maxwordlen);

int main(int argc,char **argv)
{
	FILE *f;

	char filename[FILENAMELEN];
	int maxwordlen;

	/*while (1)
	{
		printf(">Введите имя файла.\n");
		scanf("%s",filename);
		f=fopen(filename,"r");

		if (f!=NULL)
			break;

		printf("===Неверное имя файла.===\n");
	}*/


	f=fopen(argv[1],"r");
	if (f==NULL)
	{
		fprintf(stderr, "===Имя файла не существует.===\n");
		return 1;
	}

	/*while (1)
	{	
		printf(">Введите максимальное количество букв (1..256).\n");
		scanf("%i",&maxwordlen);

		if ((maxwordlen>=1)&&(maxwordlen<=256))
			break;

		printf("===Неверная длина слова.===\n");
	}*/

	if ((atoi(argv[2])<0)||(atoi(argv[2])>256))
	{
		fprintf(stderr, "===Некорректное число букв.===\n");
		return 1;
	}

	filefilter(f,atoi(argv[2]));
	fclose(f);
	return 0;
}

int filefilter(FILE*f,int maxwordlen)
{
	int i=0,curmas=0;
	char letter=' ';
	char wordbuf [MAXLEN+1];

	while (letter!=EOF)
	{
		letter=getc(f);

		if ((letter==EOF)||(letter=='\n')||(letter==' '))
		{
			wordbuf[curmas]='\0';

			if (curmas<=maxwordlen)
			{
				if (i!=0)
				{
					putchar(' ');
				}
				printf("%s",wordbuf);
				i++;
			}

			/*if (letter==' ')
				putchar(' ');	
			else */
			if (letter=='\n')
				putchar('\n');
				
			curmas=0;
		}
		else
			wordbuf[curmas++]=letter;
	}
	return 0;
}