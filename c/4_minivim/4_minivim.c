#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#define STRLEN 256
#define COMANDLEN 21


int countofstr=0,endflag=0,charnum=0,termstr=0,termwid=0,setwrapflag=1,settabwidthflag=8,comandreadflag=1,comandrest=0,
setnameflag=0,savefileflag=0,exitflag=0,memoryerrflag=0,emptyfile=1;
char comand_buf[COMANDLEN+1];
char *comand=NULL,*filename=NULL;
FILE *infile=NULL;


typedef struct                                                             /*СТРУКТУРА СТРОКИ*/
{
	char *str;
	int stringlenght;
}stringoffile;


stringoffile *file_toram (FILE *f);
char *toram (FILE *f);
void comand_analize (stringoffile *arrofstr);
int cmpstr (char *str1, char *str2);
void arg_separation ();

void fexit (stringoffile *arrofstr);
void ffilesave (stringoffile *arrofstr);
stringoffile *fopenfile (stringoffile *arrofstr);
stringoffile *freadfile (stringoffile *arrofstr);
void fset_name();
void fhelp ();
stringoffile *freplace_substring (stringoffile *arrofstr);
stringoffile *finsert_after (stringoffile *arrofstr);
stringoffile *fdelete_range (stringoffile *arrofstr);
stringoffile *fdelete_braces (stringoffile *arrofstr);
stringoffile *finsert_symbol (stringoffile *arrofstr);
stringoffile *fedit_string (stringoffile *arrofstr);
void fprint_pages (stringoffile *arrofstr);
void fprint_range (stringoffile *arrofstr);
int fset_wrap ();
int fset_tabwidth ();


int main (int argc, char const *argv[])
{
	stringoffile *arrofstr=NULL;
	int i=0;
	char new_file[]="new_file";
	char *rptr=NULL;
	struct winsize term;

	for (i=0;i<COMANDLEN+2;i++)
		comand_buf[i]=0;

	ioctl(0,TIOCGWINSZ,&term);
	termstr=term.ws_row;
	termwid=term.ws_col;

	
	printf("WELCOM *_*\n");

	if (argc!=1)
		infile=fopen(argv[1],"r");

	if (infile==NULL)
	{
		printf("===Такого файла не существует.===\n");
		printf("===Создан пустой файл с именем 'new_file'.===\n");
		printf("===Вы можете ввести файл ещё раз\nс помощью функций 'read'/'open'.===\n");

		infile=fopen("new_file","w+");
		filename=malloc(9*sizeof(char));
		if (filename==NULL)
		{
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return -1;
		}

		for (i=0;i<9;i++)
			filename[i]=new_file[i];

		setnameflag=1;

		savefileflag=0;
	}
	else
	{
		i=0;

		while (argv[1][i]!='\0')
		{
			rptr=(char *)realloc(filename,(i+1)*sizeof(char));
			if (rptr==NULL)
			{
				free(filename);
				filename=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return -1;
			}
			else
			{
				filename=rptr;
				rptr=NULL;
			}

			filename[i]=argv[1][i];  

			i++;
		}

		rptr=(char *)realloc(filename,(i+1)*sizeof(char));
		if (rptr==NULL)
		{
			free(filename);
			filename=NULL;
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return -1;
		}
		else
		{
			filename=rptr;
			rptr=NULL;
		}

		filename[i]='\0';
		setnameflag=1;

		savefileflag=0;

		arrofstr=file_toram(infile);
	}

	comand_analize(arrofstr);

	return 0;
}

void fexit (stringoffile *arrofstr)
{
	int i=0;
	char exit_force[]="force";

	if (memoryerrflag==0)
		arg_separation();

	if (((cmpstr(comand,exit_force)==0)||((savefileflag==1)&&(comand[0]=='\0')))||(memoryerrflag==1))
	{
		for (i=0;i<countofstr;i++)
		{
			free(arrofstr[i].str);
			arrofstr[i].str=NULL;
		}
		
		if (emptyfile==0)
		{
			free(arrofstr);
			arrofstr=NULL;
		}

		fclose(infile);
		infile=NULL;

		free(comand);
		comand=NULL;

		free(filename);
		filename=NULL;

		exitflag=1;

		if (memoryerrflag==0)
			printf("BYE-BYE *_*\n");
		else
		if (memoryerrflag==1)
		{
			printf("BYE-BYE :(\n");
		}

		return;
	}
	else
	if ((comand[0]=='\0')&&(savefileflag==0))
	{
		printf("===Файл не сохранен.===\n");
		printf("===Используйте команду 'write', чтобы сохранить файл,\nи команду 'exit force', чтобы выйти без сохранения.===\n");
		return;
	}
	else
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть слово 'force'\nили аргументов не должно быть совсем===\n");
		return;
	}
}

void ffilesave (stringoffile *arrofstr)
{
	int i=0,j=0;
	FILE *outfile=NULL;


	arg_separation();

	if ((comand[0]!='"')&&(comand[0]!='\0'))
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть имя файла заключенное в кавычки\nили аргумент должен отсутствовать.===\n");
		return;
	}

	if ((comand[0]=='"')&&(comand[1]=='\0'))
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
		return;
	}

	if (comand[0]=='"')	
	{
		i=1;
		j=0;

		while (comand[i]!='\0')
		{
			comand[j]=comand[i];
			i++;
			j++;
		}
		comand[j]='\0';
	}

	i=0;
	j=0;

	while (comand[i]!='\0')
	{
		if ((comand[i]=='"')&&(comand[i+1]!='\0'))
		{
			printf("===Неверный аргумент.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументом должно быть имя файла заключенное в кавычки\nили аргумент должен отсутствовать.===\n");
			return;
		}
		i++;
	}

	
	if (i!=0)
		if (comand[i-1]!='"')
		{
			printf("===Неверный аргумент.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументом должно быть имя файла заключенное в кавычки\nили аргумент должен отсутствовать.===\n");
			return;
		}

	i=0;

	while (comand[i]!='\0')
	{
		if (comand[i]=='"')
		{
			comand[i]='\0';
			break;
		}
		i++;
	}

	if ((comand[0]=='\0')&&(setnameflag==0))
	{
		printf("===Не ассоциированно имя файла.===\n");
		printf("===Воспользуйтесь функцией 'set name'.===\n");
		return;
	}

	i=0;

	if (setnameflag==1)
	{
		fclose(infile);
		infile=NULL;
		infile=fopen(filename,"w");

		for (i=0;i<countofstr;i++)
			for (j=0;j<arrofstr[i].stringlenght;j++)
				fputc((int)arrofstr[i].str[j],infile);

		savefileflag=1;

		printf("===File with name '%s' was saved.===\n",filename);

		return;
	}
	else
	if ((setnameflag==0)&&(comand[0]!='\0'))
	{
		fclose(infile);
		infile=NULL;
		infile=fopen(comand,"w+");

		for (i=0;i<countofstr;i++)
			for (j=0;j<arrofstr[i].stringlenght;j++)
				fputc((int)arrofstr[i].str[j],infile);

		savefileflag=1;

		printf("===File with name '%s' was saved.===\n",filename);

		return;
	}
}

stringoffile *fopenfile (stringoffile *arrofstr)
{
	int i=0,j=0;
	FILE *readfile=NULL;
	char *rptr=NULL;


	arg_separation();

	if ((comand[0]!='"')||(comand[0]=='\0'))
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
		return NULL;
	}

	i=1;
	j=0;

	while (comand[i]!='\0')
	{
		comand[j]=comand[i];
		i++;
		j++;
	}
	comand[j]='\0';

	i=0;
	j=0;

	if (comand[0]=='\0')
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
		return NULL;
	}

	while (comand[i]!='\0')
	{
		if ((comand[i]=='"')&&(comand[i+1]!='\0'))
		{
			printf("===Неверный аргумент.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
			return NULL;
		}
		i++;
	}

	if (i!=0)
		if (comand[i-1]!='"')
		{
			printf("===Неверный аргумент.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
			return NULL;
		}

	i=0;

	while (comand[i]!='\0')
	{
		if (comand[i]=='"')
		{
			comand[i]='\0';
			break;
		}
		i++;
	}

	readfile=fopen(comand,"r");
	if (readfile==NULL)
	{
		printf("===Такого файла не существует.===\n");
		return NULL;
	}
	else
	{
		for (i=0;i<countofstr;i++)
		{
			free(arrofstr[i].str);
		}
		free(arrofstr);
		arrofstr=NULL;

		fclose(infile);

		infile=readfile;
		readfile=NULL;

		countofstr=0;

		i=0;
		free(filename);
		filename=NULL;

		while (comand[i]!='\0')
			{
				rptr=(char *)realloc(filename,(i+1)*sizeof(char));
				if (rptr==NULL)
				{
					free(filename);
					filename=NULL;
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return NULL;
				}
				else
				{
					filename=rptr;
					rptr=NULL;
				}

				filename[i]=comand[i];  

				i++;
			}

		rptr=(char *)realloc(filename,(i+1)*sizeof(char));
		if (rptr==NULL)
		{
			free(filename);
			filename=NULL;
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return NULL;
		}
		else
		{
			filename=rptr;
			rptr=NULL;
		}

		filename[i]='\0';
		setnameflag=1;

		savefileflag=0;
		emptyfile=1;

		arrofstr=file_toram(infile);

		printf("===File with name '%s' was opened.===\n",filename);

		return arrofstr;
	}
}

stringoffile *freadfile (stringoffile *arrofstr)
{
	int i=0,j=0;
	FILE *readfile=NULL;


	arg_separation();

	if ((comand[0]!='"')||(comand[0]=='\0'))
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
		return NULL;
	}

	i=1;
	j=0;

	while (comand[i]!='\0')
	{
		comand[j]=comand[i];
		i++;
		j++;
	}
	comand[j]='\0';

	i=0;
	j=0;

	if (comand[0]=='\0')
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
		return NULL;
	}

	while (comand[i]!='\0')
	{
		if ((comand[i]=='"')&&(comand[i+1]!='\0'))
		{
			printf("===Неверный аргумент.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
			return NULL;
		}
		i++;
	}

	if (i!=0)
		if (comand[i-1]!='"')
		{
			printf("===Неверный аргумент.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
			return NULL;
		}

	i=0;

	while (comand[i]!='\0')
	{
		if (comand[i]=='"')
		{
			comand[i]='\0';
			break;
		}
		i++;
	}

	i=0;

	readfile=fopen(comand,"r");
	if (readfile==NULL)
	{
		printf("===Такого файла не существует.===\n");
		return NULL;
	}
	else
	{
		for (i=0;i<countofstr;i++)
		{
			free(arrofstr[i].str);
			arrofstr[i].str=NULL;
		}
		free(arrofstr);
		arrofstr=NULL;

		fclose(infile);

		infile=readfile;
		readfile=NULL;

		countofstr=0;

		savefileflag=0;
		emptyfile=1;

		arrofstr=file_toram(infile);

		printf("===File was opened, name didn't memorized.===\n");

		return arrofstr;
	}
}

void fset_name()
{
	int i=0,j=0;
	char *rptr=NULL;


	arg_separation();

	if (comand[0]=='\0')
	{
		printf("===Ассоциация с файлом удалена.===\n");
		free(filename);
		filename=NULL;
		setnameflag=0;
		return;
	}

	if (comand[0]!='"')
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
		return;
	}

	i=1;
	j=0;

	while (comand[i]!='\0')
	{
		comand[j]=comand[i];
		i++;
		j++;
	}
	comand[j]='\0';

	i=0;
	j=0;

	while (comand[i]!='\0')
	{
		if ((comand[i]=='"')&&(comand[i+1]!='\0'))
		{
			printf("===Неверный аргумент.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
			return;
		}
		i++;
	}

	if (comand[i-1]!='"')
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть имя файла заключенное в кавычки.===\n");
		return;
	}

	i=0;

	while (comand[i]!='\0')
	{
		if (comand[i]=='"')
		{
			comand[i]='\0';
			break;
		}
		i++;
	}

	i=0;
	free(filename);

	while (comand[i]!='\0')
	{
		rptr=(char *)realloc(filename,(i+1)*sizeof(char));
		if (rptr==NULL)
		{
			free(filename);
			filename=NULL;
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return;
		}
		else
		{
			filename=rptr;
			rptr=NULL;
		}

		filename[i]=comand[i];  

		i++;
	}

	rptr=(char *)realloc(filename,(i+1)*sizeof(char));
	if (rptr==NULL)
	{
		free(filename);
		filename=NULL;
		printf("===Недостаточно памяти.===\n");
		memoryerrflag=1;
		return;
	}
	else
	{
		filename=rptr;
		rptr=NULL;
	}

	filename[i]='\0';
	setnameflag=1;

	printf("===С файлом ассоциированно имя '%s'.===\n",filename);

	return;
}

void fhelp ()
{
	int savecountofstr=0,i=0;
	stringoffile *helpfileptr=NULL;
	FILE *helpfile=NULL;

	
	arg_separation();

	if (comand[0]!='\0')
	{
		printf("===Такой команды не существует.===\n");
		printf("===Введите команду заново.===\n");
		return;
	}
	else
	{
		helpfile=fopen("help_minivim","r");

		savecountofstr=countofstr;
		countofstr=0;
		helpfileptr=file_toram(helpfile);

		fprint_pages(helpfileptr);

		for (i=0;i<countofstr;i++)
		{
			free(helpfileptr[i].str);
			helpfileptr[i].str=NULL;
		}
		free(helpfileptr);
		helpfileptr=NULL;

		fclose(helpfile);
		helpfile=NULL;

		countofstr=savecountofstr;
	}
	return;
}

stringoffile *freplace_substring (stringoffile *arrofstr)
{
	int i=0,j=0,k=0,l=0,pos=0,arg1=0,arg2=0,argflag=0,str1len=0,str2len=0,len=0,positionflag=0,symbolflag=0;
	char *strarg1=NULL,*strarg2=NULL,*ptr1=NULL,*ptr2=NULL,*rptr=NULL;
	stringoffile *rarrofstr=NULL;


	arg_separation();

	if (((comand[0]>'9')||(comand[0]<'1'))&&(comand[0]!='"'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
		return arrofstr;
	}

	
	if (comand[0]!='"')
	{
		arg1=atoi(comand);
		argflag=1;

		if ((arg1>countofstr)||(arg1<0))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
			return arrofstr;
		}
	}
	else
	{
		arg1=1;
		arg2=countofstr;
	}

	i=0;

	if (argflag==1)
	{
		while (comand[i]!=' ')
		{
			if ((comand[i]=='\0')||((comand[0]>'9')||(comand[0]<'0')))
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
				return arrofstr;
			}

			i++;
		}

		comandrest=i;
		arg_separation();

		if (comand[0]!='"')
		{
			arg2=atoi(comand);
			argflag=2;

			if ((arg2>countofstr)||(arg2<0)||(arg2<arg1))
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
				return arrofstr;
			}
		}
		else
		{
			arg2=countofstr;
		}

		i=0;

		if (argflag==2)
		{
			while (comand[i]!=' ')
			{
				if ((comand[i]=='\0')||((comand[0]>'9')||(comand[0]<'0')))
				{
					printf("===Неверные аргументы.===\n");
					printf("===Введите команду заново.===\n");
					printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
					return arrofstr;
				}

				i++;
			}

			comandrest=i;
			arg_separation();
		}
	}

	if (comand[0]!='"')
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
		return arrofstr;
	}

	i=0;
	j=1;

	while (comand[j]!='\0')
	{
		comand[i]=comand[j];

		i++;
		j++;
	}
	comand[i]='\0';

	if ((comand[0]=='"')||(comand[0]=='\0'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
		return arrofstr;
	}

	i=0;
	j=0;

	rptr=(char *)realloc(strarg1,(j+1)*sizeof(char));
	if (rptr==NULL)
	{
		free(strarg1);
		strarg1=NULL;
		printf("===Недостаточно памяти.===\n");
		memoryerrflag=1;
		return NULL;
	}
	else
	{
		strarg1=rptr;
		rptr=NULL;
	}

	if (comand[i]=='\\')
		{
			if (comand[i+1]=='t')
			{
				strarg1[j]='\t';
				i++;
			}
			else
			if (comand[i+1]=='\\')
			{
				strarg1[j]='\\';
				i++;
			}
			else
			if (comand[i+1]=='r')
			{
				strarg1[j]='\r';
				i++;
			}
			else
			if (comand[i+1]=='"')
			{
				strarg1[j]='"';
				i++;
			}
			/*else
			if (comand[i+1]=='n')
			{
				strarg1[j]='\n';
				i++;
			}*/
			else
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
				free(strarg1);
				strarg1=NULL;
				return arrofstr;
			}
		}
		else
		{
			strarg1[j]=comand[i];
		}

	i++;
	j++;

	while ((comand[i-1]=='\\')||(comand[i]!='"'))
	{
		rptr=(char *)realloc(strarg1,(j+1)*sizeof(char));
		if (rptr==NULL)
		{
			free(strarg1);
			strarg1=NULL;
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return NULL;
		}
		else
		{
			strarg1=rptr;
			rptr=NULL;
		}

		if (comand[i]=='\\')
		{
			if (comand[i+1]=='t')
			{
				strarg1[j]='\t';
				i++;
			}
			else
			if (comand[i+1]=='\\')
			{
				strarg1[j]='\\';
				i++;
			}
			else
			if (comand[i+1]=='r')
			{
				strarg1[j]='\r';
				i++;
			}
			else
			if (comand[i+1]=='"')
			{
				strarg1[j]='"';
				i++;
			}
			/*else
			if (comand[i+1]=='n')
			{
				strarg1[j]='\n';
				i++;
			}*/
			else
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
				free(strarg1);
				strarg1=NULL;
				return arrofstr;
			}
		}
		else
		{
			strarg1[j]=comand[i];
		}

		i++;
		j++;
	}

	rptr=(char *)realloc(strarg1,(j+1)*sizeof(char));
	if (rptr==NULL)
	{
		free(strarg1);
		strarg1=NULL;
		printf("===Недостаточно памяти.===\n");
		memoryerrflag=1;
		return NULL;
	}
	else
	{
		strarg1=rptr;
		rptr=NULL;
	}
	strarg1[j]='\0';

	str1len=j+1;

	if ((strarg1[0]=='^')&&(strarg1[1]=='\0'))
	{
		symbolflag=1;
	}
	else
	if ((strarg1[0]=='$')&&(strarg1[1]=='\0'))
	{
		symbolflag=2;
	}

	comandrest=i+1;
	arg_separation();

	if (comand[0]!='"')
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
		free(strarg1);
		strarg1=NULL;
		return arrofstr;
	}

	i=0;
	j=1;

	while (comand[j]!='\0')
	{
		comand[i]=comand[j];

		i++;
		j++;
	}
	comand[i]='\0';

	if (i!=0)
	{
		if (comand[i-1]!='"')
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
			free(strarg1);
			strarg1=NULL;
			return arrofstr;
		}
	}

	if ((comand[0]=='"')||(comand[0]=='\0'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
		free(strarg1);
		strarg1=NULL;
		return arrofstr;
	}

	i=0;
	j=0;

	rptr=(char *)realloc(strarg2,(j+1)*sizeof(char));
	if (rptr==NULL)
	{
		free(strarg2);
		strarg2=NULL;
		free(strarg1);
		strarg1=NULL;
		printf("===Недостаточно памяти.===\n");
		memoryerrflag=1;
		return NULL;
	}
	else
	{
		strarg2=rptr;
		rptr=NULL;
	}

	if (comand[i]=='\\')
		{
			if (comand[i+1]=='t')
			{
				strarg2[j]='\t';
				i++;
			}
			else
			if (comand[i+1]=='\\')
			{
				strarg2[j]='\\';
				i++;
			}
			else
			if (comand[i+1]=='r')
			{
				strarg2[j]='\r';
				i++;
			}
			else
			if (comand[i+1]=='"')
			{
				strarg2[j]='"';
				i++;
			}
			/*else
			if (comand[i+1]=='n')
			{
				strarg2[j]='\n';
				i++;
			}*/
			else
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
				free(strarg1);
				strarg1=NULL;
				free(strarg2);
				strarg2=NULL;
				return arrofstr;
			}
		}
		else
		{
			strarg2[j]=comand[i];
		}


	i++;
	j++;

	while ((comand[i-1]=='\\')||(comand[i]!='"'))
	{
		rptr=(char *)realloc(strarg2,(j+1)*sizeof(char));
		if (rptr==NULL)
		{
			free(strarg2);
			strarg2=NULL;
			free(strarg1);
			strarg1=NULL;
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return NULL;
		}
		else
		{
			strarg2=rptr;
			rptr=NULL;
		}

		if (comand[i]=='\\')
		{
			if (comand[i+1]=='t')
			{
				strarg2[j]='\t';
				i++;
			}
			else
			if (comand[i+1]=='\\')
			{
				strarg2[j]='\\';
				i++;
			}
			else
			if (comand[i+1]=='r')
			{
				strarg2[j]='\r';
				i++;
			}
			else
			if (comand[i+1]=='"')
			{
				strarg2[j]='"';
				i++;
			}
			/*else
			if (comand[i+1]=='n')
			{
				strarg2[j]='\n';
				i++;
			}*/
			else
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть два числа больше 0,\nпричём первое число должно быть меньше или равно второму,\nи две строки заключенный в кавычки.===\n");
				free(strarg1);
				strarg1=NULL;
				free(strarg2);
				strarg2=NULL;
				return arrofstr;
			}
		}
		else
		{
			strarg2[j]=comand[i];
		}

		i++;
		j++;
	}
	rptr=(char *)realloc(strarg2,(j+1)*sizeof(char));
	if (rptr==NULL)
	{
		free(strarg2);
		strarg2=NULL;
		free(strarg1);
		strarg1=NULL;
		printf("===Недостаточно памяти.===\n");
		memoryerrflag=1;
		return NULL;
	}
	else
	{
		strarg2=rptr;
		rptr=NULL;
	}
	strarg2[j]='\0';
	
	str2len=j+1;

	j=0;
	pos=0;

	for (i=(arg1-1);i<arg2;i++)
	{
		pos=i+j;

		ptr1=arrofstr[pos].str;
		len=arrofstr[pos].stringlenght;

		if (symbolflag==0)
		{
			ptr1[len-1]='\0';
			ptr2=strstr(ptr1,strarg1);
			ptr1[len-1]='\n';
		}
		else
		if (symbolflag==1)
		{
			ptr2=ptr1;
		}
		else
		if (symbolflag==2)
		{
			ptr2=&ptr1[len-1];
		}

		while (ptr2!=NULL)
		{
			k=0;

			while (ptr2[k]!='\n')
			{
				k++;
			}
			k++;
			positionflag=len-k;
			
			k=0;

			while (strarg2[k]!='\n')
			{
				k++;

				if (k==str2len-1)
				{
					break;
				}
			}
			k++;

			if (symbolflag==0)
			{
				l=0;

				while (ptr2[l+str1len-1]!='\n')
				{
					ptr2[l]=ptr2[l+str1len-1];
					l++;
				}
				ptr2[l]='\n';
			}

			if (symbolflag==1)
			{
				l=len+str2len-1;
			}
			else
			{
				l=len-str1len+str2len;
			}

			rptr=(char *)realloc(ptr1,l*sizeof(char));
			if (rptr==NULL)
			{
				free(ptr1);
				ptr1=NULL;
				free(strarg2);
				strarg2=NULL;
				free(strarg1);
				strarg1=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				ptr1=rptr;
				rptr=NULL;
			}

			len=l;

			while (l!=(positionflag+str2len-1))
			{
				ptr1[l-1]=ptr1[l-str2len];

				l--;
			}

			for (l=0;l<k-1;l++)
			{
				ptr1[positionflag+l]=strarg2[l];
			}

			if (symbolflag==2)
			{
				len++;

				rptr=(char *)realloc(ptr1,len*sizeof(char));
				if (rptr==NULL)
				{
					free(ptr1);
					ptr1=NULL;
					free(strarg2);
					strarg2=NULL;
					free(strarg1);
					strarg1=NULL;
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return NULL;
				}
				else
				{
					ptr1=rptr;
					rptr=NULL;
				}

				ptr1[len-1]='\n';
			}

			rptr=&ptr1[positionflag+str2len-1];

			ptr1[len-1]='\0';
			ptr2=strstr(rptr,strarg1);
			ptr1[len-1]='\n';

			arrofstr[pos].str=ptr1;
			arrofstr[pos].stringlenght=len;
		}
		ptr1=NULL;
	}
	free(strarg1);
	strarg1=NULL;
	free(strarg2);
	strarg2=NULL;

	printf("===Substrings were successfully replaced.===\n");

	return arrofstr;
}

stringoffile *finsert_after (stringoffile *arrofstr)
{
	int i=0,j=0,k=0,arg1=0,letter=0,multistrflag=0,newstrflag=0,spaceflag=0,firstinflag=0;
	stringoffile insertstr;
	char *ptr=NULL;
	char *rptr=NULL;
	stringoffile *rarrofstr=NULL;

	arg_separation();

	if (((comand[0]>'9')||(comand[0]<'0'))&&(comand[0]!='"'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
		return arrofstr;
	}

	i=0;

	if ((comand[0]>='0')&&(comand[0]<='9'))
	{
		arg1=atoi(comand);

		while (comand[i]!=' ')
		{
			if ((comand[i]>'9')||(comand[i]<'0'))
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
				return arrofstr;
			}
			i++;
		}
	}
	else
	{
		arg1=-1;
	}

	if (arg1>countofstr)
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
		return arrofstr;
	}

	i=0;

	if (arg1!=-1)
	{
		while ((comand[i]!=' '))
		{
			i++;
		}

		comandrest=i;
		arg_separation();
	}

	if (comand[0]!='"')
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
		return arrofstr;
	}

	if (comand[1]!='"')
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
		return arrofstr;
	}

	if (comand[2]=='"')
	{
		multistrflag=1;
		i=3;
	}
	else
	{
		multistrflag=0;
		i=2;
	}

	j=0;

	while (comand[i]!='\0')
	{
		comand[j]=comand[i];
		i++;
		j++;
	}
	comand[j]='\0';

	if ((comand[0]=='\0')&&(multistrflag==1))
	{
		free(comand);
		comand=NULL;

		i=-1;

		letter=(int)getchar();	
		while (letter!='\n')
		{
			if (((letter==' ')||(letter=='	'))&&(spaceflag==0))
			{
				i++;
				rptr=(char *)realloc(comand,(i+1)*sizeof(char));
				if (rptr==NULL)
				{
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return NULL;
				}
				else
				{
					comand=rptr;
					rptr=NULL;
				}

				comand[i]=' ';
				spaceflag=1;
			}
			else
			if ((letter!=' ')&&(letter!='	'))
			{
				i++;
				rptr=(char *)realloc(comand,(i+1)*sizeof(char));
				if (rptr==NULL)
				{
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return NULL;
				}
				else
				{
					comand=rptr;
					rptr=NULL;
				}

				comand[i]=letter;
				spaceflag=0;
			}

			letter=(int)getchar();		
		}

		i++;
		rptr=(char *)realloc(comand,(i+1)*sizeof(char));
		if (rptr==NULL)
		{
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return NULL;
		}
		else
		{
			comand=rptr;
			rptr=NULL;
		}

		comand[i]='\0';
	}
	else
	if (comand[0]=='"')
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
		return arrofstr;
	}

	i=0;
	j=0;
	k=0;

	if ((multistrflag==0)&&(comand[0]=='\0'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
		return arrofstr;
	}

	if (multistrflag==0)                                                   /*ОДНОСТРОЧНЫЙ РЕЖИМ*/
	{
		while (comand[i]!='\0')
		{
			if ((comand[i]=='\\')&&(comand[i+1]!='n')&&(comand[i+1]!='t')&&(comand[i+1]!='\\')&&(comand[i+1]!='"')&&(comand[i+1]!='r')&&((i!=0)&&comand[i-1]!='\\'))
			{	
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
				return arrofstr;
			}

			i++;
		}

		if (i-3>=0)
		{
			if ((comand[i-1]!='"')||(comand[i-2]!='"'))
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
				return arrofstr;
			}
		}
		else
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
			return arrofstr;
		}

		comand[i-2]='\0';

		if (comand[0]=='\0')
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
			return arrofstr;
		}

		if (i-4>=0)
		{
			if ((comand[i-3]=='\\')&&(comand[i-4]!='\\'))
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
				return arrofstr;
			}
		}

		i=0;

		while (comand[i]!='\0')
		{
			if (comand[0]=='"')
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
				return arrofstr;
			}

			if ((i!=0)&&(comand[i]=='"')&&(comand[i-1]!='\\'))
			{
				printf("===Неверные аргументы.===\n");
				printf("===Введите команду заново.===\n");
				printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
				return arrofstr;
			}

			i++;
		}

		i=0;
		k=0;
		j=0;

		while (1)
		{
			i=k;

			insertstr.stringlenght=0;

			while (comand[i]!='\0')
			{
				insertstr.stringlenght++;
				rptr=(char *)realloc(ptr,insertstr.stringlenght*sizeof(char));
				if (rptr==NULL)
				{
					free(ptr);
					ptr=NULL;
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return NULL;
				}
				else
				{
					ptr=rptr;
					rptr=NULL;
				}

				if (comand[i]=='\\')
				{
					if (comand[i+1]=='t')
					{
						ptr[insertstr.stringlenght-1]='\t';
						i++;
					}
					else
					if (comand[i+1]=='\\')
					{
						ptr[insertstr.stringlenght-1]='\\';
						i++;
					}
					else
					if (comand[i+1]=='r')
					{
						ptr[insertstr.stringlenght-1]='\r';
						i++;
					}
					else
					if (comand[i+1]=='"')
					{
						ptr[insertstr.stringlenght-1]='"';
						i++;
					}
					else
					if (comand[i+1]=='n')
					{
						ptr[insertstr.stringlenght-1]='\n';
						i++;
						i++;
						newstrflag=1;
						break;
					}
				}
				else
				{
					ptr[insertstr.stringlenght-1]=comand[i];
				}

				i++;
			}

			k=i;

			if (newstrflag==0)
			{
				insertstr.stringlenght++;
				rptr=(char *)realloc(ptr,insertstr.stringlenght*sizeof(char));
				if (rptr==NULL)
				{
					free(ptr);
					ptr=NULL;
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return NULL;
				}
				else
				{
					ptr=rptr;
					rptr=NULL;
				}
				ptr[insertstr.stringlenght-1]='\n';
			}

			insertstr.str=ptr;
			ptr=NULL;

			countofstr++;

			rarrofstr=(stringoffile *)realloc(arrofstr,countofstr*sizeof(stringoffile));
			if (rarrofstr==NULL)
			{
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				arrofstr=rarrofstr;
				rarrofstr=NULL;
			}

			emptyfile=0;

			if (arg1!=-1)
			{
				i=countofstr-1;

				while (i!=arg1)
				{
					arrofstr[i].str=arrofstr[i-1].str;
					arrofstr[i].stringlenght=arrofstr[i-1].stringlenght;

					i--;
				}

				arrofstr[i].str=NULL;

				arrofstr[i].str=insertstr.str;
				arrofstr[i].stringlenght=insertstr.stringlenght;

				insertstr.str=NULL;

				arg1++;
			}
			else
			{
				arrofstr[countofstr-1].str=insertstr.str;
				arrofstr[countofstr-1].stringlenght=insertstr.stringlenght;

				insertstr.str=NULL;
			}

			newstrflag=0;

			if (comand[k]=='\0')
			{
				printf("===String was successfully inserted.===\n");
				return arrofstr;
			}
		}
	}
	else
	if (multistrflag==1)                                                   /*МНОГОСТРОЧНЫЙ РЕЖИМ*/
	{
		while (1)
		{
			if (firstinflag==0)
			{
				while (comand[i]!='\0')
				{
					if (comand[0]=='"')
					{
						printf("===Неверные аргументы.===\n");
						printf("===Введите команду заново.===\n");
						printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
						return arrofstr;
					}

					if ((i!=0)&&(comand[i]=='"')&&(comand[i-1]!='\\')&&(comand[i+1]!='\0')&&(comand[i+1]!='"')&&(comand[i+2]!='\0')&&(comand[i+2]!='"'))
					{
						printf("===Неверные аргументы.===\n");
						printf("===Введите команду заново.===\n");
						printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
						return arrofstr;
					}

					if ((comand[i]=='\\')&&(comand[i+1]!='n')&&(comand[i+1]!='t')&&(comand[i+1]!='\\')&&(comand[i+1]!='"')&&(comand[i+1]!='r')&&((i!=0)&&comand[i-1]!='\\'))
					{	
						printf("===Неверные аргументы.===\n");
						printf("===Введите команду заново.===\n");
						printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
						return arrofstr;
					}

					i++;
				}

				if (i-3>=0)
				{
					if ((comand[i-1]=='"')&&(comand[i-2]=='"')&&(comand[i-3]=='"'))
					{
						if (i-4>=0)
						{
							comand[i-3]='\0';
							multistrflag=0;
						}
						else
						{
							printf("===Неверные аргументы.===\n");
							printf("===Введите команду заново.===\n");
							printf("===Аргументами должны быть число больше или равное 0\nи строка, заключенная в двойные кавычки\nили несколько строк заключенные в тройные кавычки.===\n");
							return arrofstr;
						}
					}
				}

				firstinflag=1;
				i=0;
			}

			i=k;

			insertstr.stringlenght=0;

			while (comand[i]!='\0')
			{
				insertstr.stringlenght++;
				rptr=(char *)realloc(ptr,insertstr.stringlenght*sizeof(char));
				if (rptr==NULL)
				{
					free(ptr);
					ptr=NULL;
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return NULL;
				}
				else
				{
					ptr=rptr;
					rptr=NULL;
				}

				if (comand[i]=='\\')
				{
					if (comand[i+1]=='t')
					{
						ptr[insertstr.stringlenght-1]='\t';
						i++;
					}
					else
					if (comand[i+1]=='\\')
					{
						ptr[insertstr.stringlenght-1]='\\';
						i++;
					}
					else
					if (comand[i+1]=='r')
					{
						ptr[insertstr.stringlenght-1]='\r';
						i++;
					}
					else
					if (comand[i+1]=='"')
					{
						ptr[insertstr.stringlenght-1]='"';
						i++;
					}
					else
					if (comand[i+1]=='n')
					{
						ptr[insertstr.stringlenght-1]='\n';
						i++;
						i++;
						newstrflag=1;
						break;
					}

				}
				else
				{
					ptr[insertstr.stringlenght-1]=comand[i];
				}

				i++;
			}

			k=i;

			if (newstrflag==0)
			{
				insertstr.stringlenght++;
				rptr=(char *)realloc(ptr,insertstr.stringlenght*sizeof(char));
				if (rptr==NULL)
				{
					free(ptr);
					ptr=NULL;
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return NULL;
				}
				else
				{
					ptr=rptr;
					rptr=NULL;
				}
				ptr[insertstr.stringlenght-1]='\n';
			}

			insertstr.str=ptr;
			ptr=NULL;

			countofstr++;

			rarrofstr=(stringoffile *)realloc(arrofstr,countofstr*sizeof(stringoffile));
			if (rarrofstr==NULL)
			{
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				arrofstr=rarrofstr;
				rarrofstr=NULL;
			}

			emptyfile=0;

			if (arg1!=-1)
			{
				i=countofstr-1;

				while (i!=arg1)
				{
					arrofstr[i].str=arrofstr[i-1].str;
					arrofstr[i].stringlenght=arrofstr[i-1].stringlenght;

					i--;
				}

				arrofstr[i].str=NULL;

				arrofstr[i].str=insertstr.str;
				arrofstr[i].stringlenght=insertstr.stringlenght;

				insertstr.str=NULL;

				arg1++;
			}
			else
			{
				arrofstr[countofstr-1].str=insertstr.str;
				arrofstr[countofstr-1].stringlenght=insertstr.stringlenght;

				insertstr.str=NULL;
			}

			if (multistrflag==0)
			{
				printf("===Strings were successfully inserted.===\n");
				return arrofstr;
			}

			if (newstrflag==0)
			{
				while (1)
				{
					free(comand);
					comand=NULL;

					i=-1;
					k=0;

					letter=(int)getchar();	
					while (letter!='\n')
					{
						if (((letter==' ')||(letter=='	'))&&(spaceflag==0))
						{
							i++;
							rptr=(char *)realloc(comand,(i+1)*sizeof(char));
							if (rptr==NULL)
							{
								printf("===Недостаточно памяти.===\n");
								memoryerrflag=1;
								return NULL;
							}
							else
							{
								comand=rptr;
								rptr=NULL;
							}

							comand[i]=' ';
							spaceflag=1;
						}
						else
						if ((letter!=' ')&&(letter!='	'))
						{
							i++;
							rptr=(char *)realloc(comand,(i+1)*sizeof(char));
							if (rptr==NULL)
							{
								printf("===Недостаточно памяти.===\n");
								memoryerrflag=1;
								return NULL;
							}
							else
							{
								comand=rptr;
								rptr=NULL;
							}
							comand[i]=letter;
							spaceflag=0;
						}

						letter=(int)getchar();		
					}

					i++;
					rptr=(char *)realloc(comand,(i+1)*sizeof(char));
					if (rptr==NULL)
					{
						printf("===Недостаточно памяти.===\n");
						memoryerrflag=1;
						return NULL;
					}
					else
					{
						comand=rptr;
						rptr=NULL;
					}
					comand[i]='\0';

					if (i-3>=0)
					{
						if ((comand[i-1]=='"')&&(comand[i-2]=='"')&&(comand[i-3]=='"'))
						{
							if (i-4>=0)
							{
								comand[i-3]='\0';
								multistrflag=0;
								break;
							}
							else
							{
								printf("===Strings were successfully inserted.===\n");
								return arrofstr;
							}
						}
					}

					i=0;

					while (comand[i]!='\0')
					{
						if (comand[0]=='"')
						{
							printf("===Строка введена неверно.===\n");
							printf("===Введите строку заново.===\n");
							i=-1;
							break;
						}

						if ((i!=0)&&(comand[i]=='"')&&(comand[i-1]!='\\'))
						{
							printf("===Строка введена неверно.===\n");
							printf("===Введите строку заново.===\n");
							i=-1;
							break;
						}

						if ((comand[i]=='\\')&&(comand[i+1]!='n')&&(comand[i+1]!='t')&&(comand[i+1]!='\\')&&(comand[i+1]!='"')&&(comand[i+1]!='r')&&((i!=0)&&comand[i-1]!='\\'))
						{	
							printf("===Строка введена неверно.===\n");
							printf("===Введите строку заново.===\n");
							i=-1;
							break;
						}

						i++;
					}

					if (i!=-1)
					{
						break;
					}
				}
			}

			newstrflag=0;
		}
	}
}

stringoffile *fdelete_range (stringoffile *arrofstr)
{
	int letter=0,len=0,i=0,j=0,l=0,termstr1=0,countofstr1=0,arg1=0,arg2=0,argflag=0;
	char *ptr=NULL;


	arg_separation();

	if (((comand[0]>'9')||(comand[0]<'1'))&&(comand[0]!='\0'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
		return arrofstr;
	}

	while (comand[i]!='\0')
	{
		if (((comand[i]>'9')||(comand[i]<'0'))&&(comand[i]!=' '))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
			return arrofstr;
		}
		i++;
	}

	i=0;

	if (comand[0]=='\0')
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
		return arrofstr;
	}
	else
	{
		arg1=atoi(comand);

		while ((comand[i]!=' '))
		{
			if (comand[i]=='\0')
			{
				i=-1;
				break;
			}
			i++;
		}

		if (i!=-1)
		{
			comandrest=i;
			arg_separation();
			arg2=atoi(comand);
			argflag=1;
		}

		if ((((arg1>arg2)||(arg2>countofstr))&&(argflag==1))||(arg1>countofstr))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
			return arrofstr;
		}
	}

	if (argflag==0)
	{
		arg2=countofstr;
	}


	for (i=(arg1-1);i<arg2;i++)
	{
		ptr=arrofstr[i].str;
		free(ptr);
		ptr=NULL;
		arrofstr[i].str=NULL;
	}

	i=arg1-1;
	j=arg2;

	while (j!=countofstr)
	{
		arrofstr[i].str=arrofstr[j].str;
		arrofstr[i].stringlenght=arrofstr[j].stringlenght;

		i++;
		j++;
	}

	countofstr=countofstr-(arg2-(arg1-1));

	printf("===String/strings from %i to %i was/were deleted.===\n",arg1,arg2);

	return arrofstr;
}

stringoffile *fdelete_braces (stringoffile *arrofstr)
{
	int letter=0,len=0,len1=0,strtodellen=0,i=0,j=0,k=0,l=0,arg1=0,arg2=0,argflag=0,balanceflag=0,existflag=0,glueflag=0,inflag=0,mainstrflag=-1;
	char *ptr=NULL,*ptr1=NULL;
	int *strtodel=NULL,*rintptr=NULL;
	char *rptr=NULL;
	stringoffile *rarrofstr=NULL;

	arg_separation();

	if (((comand[0]>'9')||(comand[0]<'1'))&&(comand[0]!='\0'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
		return arrofstr;
	}

	while (comand[i]!='\0')
	{
		if (((comand[i]>'9')||(comand[i]<'0'))&&(comand[i]!=' '))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
			return arrofstr;
		}
		i++;
	}

	i=0;

	if (comand[0]=='\0')
	{
		arg1=1;
		arg2=countofstr;
	}
	else
	{
		arg1=atoi(comand);

		while ((comand[i]!=' '))
		{
			if (comand[i]=='\0')
			{
				i=-1;
				break;
			}
			i++;
		}

		if (i!=-1)
		{
			comandrest=i;
			arg_separation();
			arg2=atoi(comand);
			argflag=1;
		}

		if ((((arg1>arg2)||(arg2>countofstr))&&(argflag==1))||(arg1>countofstr))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
			return arrofstr;
		}
	}

	if (argflag==0)
	{
		arg2=countofstr;
	}

	for (i=(arg1-1);i<arg2;i++)
	{
		len=arrofstr[i].stringlenght;
		ptr=arrofstr[i].str;

		if ((balanceflag!=0)&&(i!=(arg1-1)))
		{
			if (mainstrflag==-1)
			{
				mainstrflag=i-1;
			}

			len1=arrofstr[mainstrflag].stringlenght;
			ptr1=arrofstr[mainstrflag].str;

			strtodellen++;
			rintptr=(int *)realloc(strtodel,strtodellen*sizeof(int));
			if (rintptr==NULL)
			{
				free(strtodel);
				strtodel=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				strtodel=rintptr;
				rintptr=NULL;
			}

			strtodel[strtodellen-1]=i;

			rptr=(char *)realloc(ptr1,(len+len1-1)*sizeof(char));
			if (rptr==NULL)
			{
				free(ptr1);
				ptr1=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				ptr1=rptr;
				rptr=NULL;
			}

			j=len1-1;
			k=0;

			while (ptr[k]!='\n')
			{
				ptr1[j]=ptr[k];
				j++;
				k++;
			}
			ptr1[j]='\n';

			arrofstr[mainstrflag].stringlenght=len+len1-1;
			arrofstr[mainstrflag].str=ptr1;

			ptr1=NULL;

			len=arrofstr[mainstrflag].stringlenght;
			ptr=arrofstr[mainstrflag].str;

			glueflag=1;
		}

		if (glueflag==0)
		{
			j=0;
		}
		else
		{
			j=len1-1;
		}

		k=0;

		while (ptr[j]!='\n')
		{
			inflag=1;

			if (ptr[j]=='{')
			{
				balanceflag++;
				existflag=1;
			}
			else
			if ((ptr[j]=='}')&&(existflag==1))
			{
				balanceflag--;
			}

			if ((balanceflag>0)||((existflag==1)&&(balanceflag==0)))
			{
				for (k=j;k<(len-1);k++)
				{
					ptr[k]=ptr[k+1];					
				}

				len--;
				j--;

				if (balanceflag==0)
				{
					existflag=0;
				}
			}

			j++;
		}

		if ((ptr[0]=='\n')&&(inflag==1)&&(glueflag==0))
		{
			inflag=0;

			strtodellen++;
			rintptr=(int *)realloc(strtodel,strtodellen*sizeof(int));
			if (rintptr==NULL)
			{
				free(strtodel);
				strtodel=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				strtodel=rintptr;
				rintptr=NULL;
			}
			strtodel[strtodellen-1]=i;
			ptr=NULL;

			if (balanceflag==0)
				{
					mainstrflag=-1;
				}
		}
		else
		{
			if (glueflag==1)
			{
				arrofstr[mainstrflag].stringlenght=len;
				arrofstr[mainstrflag].str=ptr;
				ptr=NULL;
				glueflag=0;

				if (balanceflag==0)
				{
					mainstrflag=-1;
				}
			}
			else
			if (glueflag==0)	
			{
				arrofstr[i].stringlenght=len;
				arrofstr[i].str=ptr;
				ptr=NULL;

				if (balanceflag==0)
				{
					mainstrflag=-1;
				}
			}	
		}
	}

	for (i=(strtodellen-1);i>-1;i--)
	{
		k=strtodel[i];
		free(arrofstr[k].str);
		arrofstr[k].str=NULL;

		l=k;
		j=k+1;

		while (j!=countofstr)
		{
			arrofstr[l].str=arrofstr[j].str;
			arrofstr[l].stringlenght=arrofstr[j].stringlenght;

			l++;
			j++;
		}

		countofstr--;
	}

	free(strtodel);
	strtodel=NULL;

	printf("===Text between braces was deleted.===\n");

	return arrofstr;
}

stringoffile *finsert_symbol (stringoffile *arrofstr)
{
	int i=0,j=0,arg1=0,arg2=0,arg3=0,len=0,spacecount=0,symbolflag=0;
	char *ptr=NULL;
	stringoffile str2;
	stringoffile *arrofstr2=NULL;
	char *rptr=NULL;
	stringoffile *rarrofstr=NULL;


	while (comand[i]!='\0')
	{
		if ((comand[i]=='\\')&&(comand[i+1]=='#'))
			symbolflag=1;

		i++;
	}

	i=0;

	arg_separation();
	
	if (((comand[0]>'9')||(comand[0]<'1'))&&(comand[0]!='\0'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
		printf("===Введите '0' на месте второго аргумента,\nчтобы добавить символ в начало строки.===\n");
		return arrofstr;
	}

	while (comand[i]!='\0')
	{
		if (comand[i]==' ')
			spacecount++;

		i++;
	}

	if (spacecount!=2)
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
		printf("===Введите '0' на месте второго аргумента,\nчтобы добавить символ в начало строки.===\n");
		return arrofstr;
	}

	i=0;
	spacecount=0;

	while (comand[i]!='\0')
	{
		if ((((comand[i]>'9')||(comand[i]<'0'))&&(comand[i]!=' ')&&(comand[i]!='\\'))&&(comand[i+1]!='\0'))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
			printf("===Введите '0' на месте второго аргумента,\nчтобы добавить символ в начало строки.===\n");
			return arrofstr;
		}
		i++;
	} 

	i=0;

	while (comand[i]!=' ')
	{
		if ((comand[i]>'9')||(comand[i]<'0')||(comand[0]=='0'))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
			printf("===Введите '0' на месте второго аргумента,\nчтобы добавить символ в начало строки.===\n");
			return arrofstr;
		}
		i++;
	}

	arg1=atoi(comand);
	comandrest=i;
	arg_separation();

	i=0;

	while (comand[i]!=' ')
	{
		if ((comand[i]>'9')||(comand[i]<'0'))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
			printf("===Введите '0' на месте второго аргумента,\nчтобы добавить символ в начало строки.===\n");
			return arrofstr;
		}
		i++;
	}

	arg2=atoi(comand);
	comandrest=i;
	arg_separation();

	i=0;

	if (symbolflag==1)
	{
		comand[1]='#';
		comand[2]='\0';
	}

	if ((comand[0]=='\\')&&(comand[1]!='\0')&&(comand[1]!='#')&&(comand[1]!='t')&&(comand[1]!='n'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
		printf("===Введите '0' на месте второго аргумента,\nчтобы добавить символ в начало строки.===\n");
		return arrofstr;
	}
	else
	if (comand[1]!='\0')
	{
		if (comand[1]=='#')
			arg3='#';
		else
		if (comand[1]=='t')
			arg3='\t';
		else
		if (comand[1]=='n')
			arg3='\n';
	}
	else
	if (comand[1]=='\0')
	{
		arg3=comand[0];
	}

	if (arg1>countofstr)
	{
		printf("===Строки с таким номером в файле не существует.===\n");
		printf("===Введите команду заново.===\n");
		return arrofstr;
	}


	len=arrofstr[arg1-1].stringlenght;
	ptr=arrofstr[arg1-1].str;

	if (arg3!='\n')
	{
		if (arg2<len)
		{	
			len++;
			i=len;

			rptr=(char *)realloc(ptr,len*sizeof(char));
			if (rptr==NULL)
			{
				free(ptr);
				ptr=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				ptr=rptr;
				rptr=NULL;
			}

			while (i!=(arg2+1))
			{
				ptr[i-1]=ptr[i-2];

				i--;
			}

			ptr[arg2]=(char)arg3;

			arrofstr[arg1-1].stringlenght=len;
			arrofstr[arg1-1].str=ptr;
		}
		else
		{
			len++;
			i=len;

			rptr=(char *)realloc(ptr,len*sizeof(char));
			if (rptr==NULL)
			{
				free(ptr);
				ptr=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				ptr=rptr;
				rptr=NULL;
			}

			ptr[i-1]=ptr[i-2];

			ptr[i-2]=(char)arg3;

			arrofstr[arg1-1].stringlenght=len;
			arrofstr[arg1-1].str=ptr;
		}
	}
	else
	{
		if (arg2<len)
		{	
			len++;
			i=len;

			rptr=(char *)realloc(ptr,len*sizeof(char));
			if (rptr==NULL)
			{
				free(ptr);
				ptr=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				ptr=rptr;
				rptr=NULL;
			}

			while (i!=(arg2+1))
			{
				ptr[i-1]=ptr[i-2];

				i--;
			}

			ptr[arg2]=(char)arg3;

			arrofstr[arg1-1].str=ptr;
		}
		else
		{
			len++;
			i=len;

			rptr=(char *)realloc(ptr,len*sizeof(char));
			if (rptr==NULL)
			{
				free(ptr);
				ptr=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				ptr=rptr;
				rptr=NULL;
			}

			ptr[i-1]=ptr[i-2];

			ptr[i-2]=(char)arg3;

			arrofstr[arg1-1].str=ptr;
		}

		i=0;

		while (ptr[i]!='\n')
		{
			i++;
		} 

		arrofstr[arg1-1].stringlenght=i+1;

		str2.str=NULL;
		str2.stringlenght=0;
		i++;
		while (ptr[i]!='\n')
		{
			str2.stringlenght++;
			rptr=(char *)realloc(str2.str,str2.stringlenght*sizeof(char));
			if (rptr==NULL)
			{
				free(str2.str);
				str2.str=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				str2.str=rptr;
				rptr=NULL;
			}

			str2.str[str2.stringlenght-1]=ptr[i];

			i++;
		}
		str2.stringlenght++;
		rptr=(char *)realloc(str2.str,str2.stringlenght*sizeof(char));
		if (rptr==NULL)
		{
			free(str2.str);
			str2.str=NULL;
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return NULL;
		}
		else
		{
			str2.str=rptr;
			rptr=NULL;
		}
		str2.str[str2.stringlenght-1]='\n';

		countofstr++;

		arrofstr2=(stringoffile *)realloc(arrofstr,countofstr*sizeof(stringoffile));
		if (arrofstr2==NULL)
		{
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return arrofstr;
		}
		else
		{
			arrofstr=arrofstr2;
			arrofstr2=NULL;
		}

		i=countofstr;
		while (i!=(arg1+1))
		{
			arrofstr[i-1].str=arrofstr[i-2].str;
			arrofstr[i-1].stringlenght=arrofstr[i-2].stringlenght;

			i--;
		}

		arrofstr[arg1].str=str2.str;
		arrofstr[arg1].stringlenght=str2.stringlenght;
	}

	ptr=NULL;

	printf("===Symbol was successfully inserted in %i string on %i position.===\n",arg1,arg2);

	return arrofstr;
}

stringoffile *fedit_string (stringoffile *arrofstr)
{
	int i=0,j=0,arg1=0,arg2=0,arg3=0,len=0,spacecount=0,symbolflag=0;
	char *ptr=NULL;
	stringoffile str2;
	stringoffile *arrofstr2=NULL;
	char *rptr=NULL;
	stringoffile *rarrofstr=NULL;


	while (comand[i]!='\0')
	{
		if ((comand[i]=='\\')&&(comand[i+1]=='#'))
			symbolflag=1;

		i++;
	}

	i=0;

	arg_separation();
	
	if (((comand[0]>'9')||(comand[0]<'1'))&&(comand[0]!='\0'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
		return arrofstr;
	}

	while (comand[i]!='\0')
	{
		if (comand[i]==' ')
			spacecount++;

		i++;
	}

	if (spacecount!=2)
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
		return arrofstr;
	}

	i=0;
	spacecount=0;

	while (comand[i]!='\0')
	{
		if ((((comand[i]>'9')||(comand[i]<'0'))&&(comand[i]!=' ')&&(comand[i]!='\\'))&&(comand[i+1]!='\0'))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
			return arrofstr;
		}
		i++;
	} 

	i=0;

	while (comand[i]!=' ')
	{
		if ((comand[i]>'9')||(comand[i]<'0')||(comand[0]=='0'))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
			return arrofstr;
		}
		i++;
	}

	arg1=atoi(comand);
	comandrest=i;
	arg_separation();

	i=0;

	while (comand[i]!=' ')
	{
		if ((comand[i]>'9')||(comand[i]<'0')||(comand[0]=='0'))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
			return arrofstr;
		}
		i++;
	}

	arg2=atoi(comand);
	comandrest=i;
	arg_separation();

	i=0;

	if (symbolflag==1)
	{
		comand[1]='#';
		comand[2]='\0';
	}

	if ((comand[0]=='\\')&&(comand[1]!='\0')&&(comand[1]!='#')&&(comand[1]!='t')&&(comand[1]!='n'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть два числа больше 0 и символ.===\n");
		return arrofstr;
	}
	else
	if (comand[1]!='\0')
	{
		if (comand[1]=='#')
			arg3='#';
		else
		if (comand[1]=='t')
			arg3='\t';
		else
		if (comand[1]=='n')
			arg3='\n';
	}
	else
	if (comand[1]=='\0')
	{
		arg3=comand[0];
	}

	if (arg1>countofstr)
	{
		printf("===Строки с таким номером в файле не существует.===\n");
		printf("===Введите команду заново.===\n");
		return arrofstr;
	}


	len=arrofstr[arg1-1].stringlenght;
	ptr=arrofstr[arg1-1].str;

	if (arg2>(len-1))
	{
		printf("===В строке нет символа с таким номером.===\n");
		printf("===Введите команду заново.===\n");
		ptr=NULL;
		return arrofstr;
	}

	ptr[arg2-1]=(char)arg3;

	if (arg3=='\n')
	{
		i=0;

		while (ptr[i]!='\n')
		{
			i++;
		} 

		arrofstr[arg1-1].stringlenght=i+1;

		str2.str=NULL;
		str2.stringlenght=0;
		i++;
		while (ptr[i]!='\n')
		{
			str2.stringlenght++;
			rptr=(char *)realloc(str2.str,str2.stringlenght*sizeof(char));
			if (rptr==NULL)
			{
				free(str2.str);
				str2.str=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return NULL;
			}
			else
			{
				str2.str=rptr;
				rptr=NULL;
			}

			str2.str[str2.stringlenght-1]=ptr[i];

			i++;
		}
		str2.stringlenght++;
		rptr=(char *)realloc(str2.str,str2.stringlenght*sizeof(char));
		if (rptr==NULL)
		{
			free(str2.str);
			str2.str=NULL;
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return NULL;
		}
		else
		{
			str2.str=rptr;
			rptr=NULL;
		}
		str2.str[str2.stringlenght-1]='\n';

		countofstr++;

		arrofstr2=(stringoffile *)realloc(arrofstr,countofstr*sizeof(stringoffile));
		if (arrofstr2==NULL)
		{
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return arrofstr;
		}
		else
		{
			arrofstr=arrofstr2;
			arrofstr2=NULL;
		}

		i=countofstr;
		while (i!=(arg1+1))
		{
			arrofstr[i-1].str=arrofstr[i-2].str;
			arrofstr[i-1].stringlenght=arrofstr[i-2].stringlenght;

			i--;
		}

		arrofstr[arg1].str=str2.str;
		arrofstr[arg1].stringlenght=str2.stringlenght;
	}

	ptr=NULL;

	printf("===Symbol was successfully replaced in %i string on %i position.===\n",arg1,arg2);

	return arrofstr;
}

void fprint_pages (stringoffile *arrofstr)
{
	int letter=0,len=0,i=0,j=0,k=0,l=0,termstr1=0,countofstr1=0,printstep=0,endoffuncflag=0,wrapflag=0,errflag=0,
	stringwrapflag=0,endofstrflag=0,shiftflag=0,printflag=0;
	char *ptr=NULL;
	struct winsize term;


	ioctl(0,TIOCGWINSZ,&term);
	termstr=term.ws_row;
	termwid=term.ws_col;

	arg_separation();

	if (comand[0]=='\0')
	{
		termstr1=termstr-1;
		countofstr1=countofstr;

		if (countofstr1<termstr1)
			{
				termstr1=countofstr1;
			}

		while (1)
		{
			printflag=0;

			for (i=0;i<termstr1;i++)
			{
				len=arrofstr[i+printstep].stringlenght;
				ptr=arrofstr[i+printstep].str;

				letter=0;
				endofstrflag=0;

				if ((setwrapflag==0)&&(termwid<len))
				{
					len=termwid;
					wrapflag=1;

					if ((shiftflag>0)&&(len+shiftflag<=arrofstr[i+printstep].stringlenght))
					{
						len=len+shiftflag;
						if (len==arrofstr[i+printstep].stringlenght)
						{
							printflag=1;
						}
					}
					else
					if ((shiftflag>0)&&(len+shiftflag>arrofstr[i+printstep].stringlenght))
					{
						len=arrofstr[i+printstep].stringlenght;
						printflag=1;
					}
				}

				if ((setwrapflag==1)&&(termwid<len))
				{
					for (j=0;j<len;j++)
					{
						if (ptr[j]=='	')
							letter=letter+settabwidthflag;
						else
							letter++;
					}

					while (letter>termwid)
					{
						letter=letter-termwid;
						l++;
					}

					stringwrapflag=1;
				}

				letter=0;

				if ((stringwrapflag==1)&&((i+l+1)>=termstr))
					break;

				for (j=shiftflag;j<len;j++)
				{
					if (ptr[j]=='	')
					{
						for (k=0;k<settabwidthflag;k++)
						{
							letter++;

							if ((setwrapflag==0)&&(letter==len))
							{
								endofstrflag=1;
								break;
							}

							putchar(' ');
						}
					}
					else
					{
						letter++;

						if ((setwrapflag==0)&&(letter==len))
						{
							endofstrflag=1;
							break;
						}

						putchar(ptr[j]);
					}

					if (endofstrflag==1)
						break;
				}

				if (((wrapflag==1)||(endofstrflag==1))&&(printflag==0))
				{
					putchar('\n');
					wrapflag=0;
				}
			}

			if ((countofstr1-i)<=0)
			{
				endoffuncflag=1;
				break;
			}

			printflag=0;

			if (endoffuncflag!=1)                                          /*ОБРАБОТЧИК ОПЕРАЦИЙ*/
				while (1)
				{
					printf(">");
					letter=getchar();

					if (letter=='q')
					{
						endoffuncflag=1;

						while (letter!='\n')
						{
							letter=getchar();
							if (letter!='\n')
								errflag=1;
						}

						if (errflag!=1)
							break;
					}
					else
					if (letter==' ')
					{
						while (letter!='\n')
						{
							letter=getchar();
							if (letter!='\n')
								errflag=1;
						}

						if (errflag!=1)
							break;

						shiftflag=0;
					}
					else
					if ((setwrapflag==0)&&(letter=='>'))
					{
						while (letter!='\n')
						{
							letter=getchar();
							if (letter!='\n')
								errflag=1;
						}

						if (errflag==0)
						{
							shiftflag++;
							break;
						}
					}
					else
					if ((setwrapflag==0)&&(letter=='<'))
					{
						while (letter!='\n')
						{
							letter=getchar();
							if (letter!='\n')
								errflag=1;
						}

						if ((errflag==0)&&(shiftflag!=0))
						{
							shiftflag--;
							printflag=1;
							break;
						}
						else
						{
							errflag=1;
						}
					}
					else
					{
						errflag=2;
					}

					if (errflag!=0)
					{
						printf("===Такой операции не существует.===\n");
						printf("===Введите операцию заново.===\n");
						if (errflag==2)
							while (letter!='\n')
								letter=getchar();

						errflag=0;
						endoffuncflag=0;
					}
				}

			if ((shiftflag==0)&&(printflag==0))
			{
				countofstr1=countofstr1-i;
				printstep=printstep+i;

				l=0;
				stringwrapflag=0;

				if (countofstr1<termstr1)
				{
					termstr1=countofstr1;
				}

				if (termstr1<=0)
				{
					endoffuncflag=1;
					break;
				}
			}

			if (endoffuncflag==1)
			{
				break;
			}
		}
		ptr=NULL;
		return;
	}
	else
	{
		printf("===Такой команды не существует.===\n");
		printf("===Введите команду заново.===\n");
		return;
	}
}

void fprint_range (stringoffile *arrofstr)
{
	int letter=0,len=0,i=0,j=0,l=0,termstr1=0,countofstr1=0,k=0,printstep=0,endoffuncflag=0,arg1=0,arg2=0,
	argflag=0,wrapflag=0,errflag=0,stringwrapflag=0,endofstrflag=0,shiftflag=0,printflag=0;
	char *ptr=NULL;
	struct winsize term;


	ioctl(0,TIOCGWINSZ,&term);
	termstr=term.ws_row;
	termwid=term.ws_col;

	arg_separation();

	if (((comand[0]>'9')||(comand[0]<'1'))&&(comand[0]!='\0'))
	{
		printf("===Неверные аргументы.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
		return;
	}

	while (comand[i]!='\0')
	{
		if (((comand[i]>'9')||(comand[i]<'0'))&&(comand[i]!=' '))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
			return;
		}
		i++;
	}

	i=0;

	if (comand[0]=='\0')
	{
		fprint_pages(arrofstr);
		return;
	}
	else
	{
		arg1=atoi(comand);

		while ((comand[i]!=' '))
		{
			if (comand[i]=='\0')
			{
				i=-1;
				break;
			}
			i++;
		}

		if (i!=-1)
		{
			comandrest=i;
			arg_separation();
			arg2=atoi(comand);
			argflag=1;
		}

		if ((((arg1>arg2)||(arg2>countofstr))&&(argflag==1))||(arg1>countofstr))
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
			return;
		}

		arg1--;

		termstr1=termstr-1;
		
		if (argflag==1)
			countofstr1=arg2-arg1;
		else
			countofstr1=countofstr-arg1;

		if (countofstr1<1)
		{
			printf("===Неверные аргументы.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументами должны быть числа больше 0,\nпричём первое число должно быть меньше второго.===\n");
			return;
		}

		if (countofstr1<termstr1)
		{
			termstr1=countofstr1;
		}

		while (1)
		{
			printflag=0;

			for (i=0;i<termstr1;i++)
			{
				len=arrofstr[i+printstep+arg1].stringlenght;
				ptr=arrofstr[i+printstep+arg1].str;

				letter=0;
				endofstrflag=0;

				if ((setwrapflag==0)&&(termwid<len))
				{
					len=termwid;
					wrapflag=1;

					if ((shiftflag>0)&&(len+shiftflag<=arrofstr[i+printstep+arg1].stringlenght))
					{
						len=len+shiftflag;
						if (len==arrofstr[i+printstep+arg1].stringlenght)
						{
							printflag=1;
						}
					}
					else
					if ((shiftflag>0)&&(len+shiftflag>arrofstr[i+printstep+arg1].stringlenght))
					{
						len=arrofstr[i+printstep+arg1].stringlenght;
						printflag=1;
					}
				}

				if ((setwrapflag==1)&&(termwid<len))
				{
					for (j=0;j<len;j++)
					{
						if (ptr[j]=='	')
							letter=letter+settabwidthflag;
						else
							letter++;
					}

					while (letter>termwid)
					{
						letter=letter-termwid;
						l++;
					}

					stringwrapflag=1;
				}

				letter=0;

				if (((i+l+1)>=termstr)&&(stringwrapflag==1))
					break;

				for (j=shiftflag;j<len;j++)
				{
					if (ptr[j]=='	')
					{
						for (k=0;k<settabwidthflag;k++)
						{
							letter++;

							if ((setwrapflag==0)&&(letter==len))
							{
								endofstrflag=1;
								break;
							}

							putchar(' ');
						}
					}
					else
					{
						letter++;

						if ((setwrapflag==0)&&(letter==len))
						{
							endofstrflag=1;
							break;
						}

						putchar(ptr[j]);
					}

					if (endofstrflag==1)
						break;
				}

				if (((wrapflag==1)||(endofstrflag==1))&&(printflag==0))
				{
					putchar('\n');
					wrapflag=0;
				}
			}

			if ((countofstr1-i)<=0)
			{
				endoffuncflag=1;
				break;
			}

			printflag=0;

			if (endoffuncflag!=1)                                          /*ОБРАБОТЧИК ОПЕРАЦИЙ*/
				while (1)
				{
					printf(">");
					letter=getchar();

					if (letter=='q')
					{
						endoffuncflag=1;

						while (letter!='\n')
						{
							letter=getchar();
							if (letter!='\n')
								errflag=1;
						}

						if (errflag!=1)
							break;
					}
					else
					if (letter==' ')
					{
						while (letter!='\n')
						{
							letter=getchar();
							if (letter!='\n')
								errflag=1;
						}

						if (errflag!=1)
							break;

						shiftflag=0;
					}
					else
					if ((setwrapflag==0)&&(letter=='>'))
					{
						while (letter!='\n')
						{
							letter=getchar();
							if (letter!='\n')
								errflag=1;
						}

						if (errflag==0)
						{
							shiftflag++;
							break;
						}
					}
					else
					if ((setwrapflag==0)&&(letter=='<'))
					{
						while (letter!='\n')
						{
							letter=getchar();
							if (letter!='\n')
								errflag=1;
						}

						if ((errflag==0)&&(shiftflag!=0))
						{
							shiftflag--;
							printflag=1;
							break;
						}
						else
						{
							errflag=1;
						}
					}
					else
					{
						errflag=2;
					}

					if (errflag!=0)
					{
						printf("===Такой операции не существует.===\n");
						printf("===Введите операцию заново.===\n");
						if (errflag==2)
							while (letter!='\n')
								letter=getchar();

						errflag=0;
						endoffuncflag=0;
					}
				}

			if ((shiftflag==0)&&(printflag==0))
			{
				countofstr1=countofstr1-i;
				printstep=printstep+i;

				l=0;
				stringwrapflag=0;

				if (countofstr1<termstr1)
				{
					termstr1=countofstr1;
				}

				if (termstr1<=0)
				{
					endoffuncflag=1;
					break;
				}
			}

			if (endoffuncflag==1)
			{
				break;
			}
		}
		ptr=NULL;
		return;
	}
}

int fset_wrap ()
{
	char set_wrap_yes[]="yes";
	char set_wrap_no[]="no";


	arg_separation();

	if (cmpstr(comand,set_wrap_yes)==0)
	{
		setwrapflag=1;
		printf("===Strings are wraping.===\n");
		return setwrapflag;
	}
	else
	if (cmpstr(comand,set_wrap_no)==0)
	{
		setwrapflag=0;
		printf("===Strings aren't wraping.===\n");
		return setwrapflag;
	}
	else
	{
		printf("===Неверный аргумент.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть слово 'yes' или слово 'no'===\n");
		setwrapflag=-1;
		return setwrapflag;
	}
}

int fset_tabwidth ()
{
	int i=0;


	arg_separation();

	if ((comand[0]>'9')||(comand[0]<'1')||(comand[0]=='\0'))
	{
		printf("===Неверный аргумент, выставлено значение по умолчанию.===\n");
		printf("===Введите команду заново.===\n");
		printf("===Аргументом должно быть число больше 0===\n");
		settabwidthflag=-1;
		return settabwidthflag;
	}

	while (comand[i]!='\0')
	{
		if ((comand[i]>'9')||(comand[i]<'0'))
		{
			printf("===Неверный аргумент, выставлено значение по умолчанию.===\n");
			printf("===Введите команду заново.===\n");
			printf("===Аргументом должно быть число больше 0===\n");
			settabwidthflag=-1;
			return settabwidthflag;
		}
		i++;
	}

	settabwidthflag=atoi(comand);

	printf("===Tabulations will be replaced with %i spaces.===\n",settabwidthflag);

	return settabwidthflag;
}

void arg_separation ()
{
	int i=0,j=0;

	i=comandrest;
	while (comand[i]!='\0')
	{
		comand[j]=comand[i];
		j++;
		i++;
	}	
	comand[j]='\0';


	i=0;
	while (comand[i]!='\0')
	{
		if (comand[i]=='#')
		{
			comand[i]='\0';
			break;
		}
		i++;
	}

	if (comand[0]!='\0')
	{
		j=1;
		while (comand[j-1]!='\0')
		{
			comand[j-1]=comand[j];
			j++;
		}
	}

	if (comand[0]!='\0')
	{
		j=j-2;
		if (comand[j-1]==' ')
			while (comand[j-1]!='\0')
			{
				comand[j-1]=comand[j];
				j++;
			}
	}

	return;
}

void comand_analize (stringoffile *arrofstr)
{
	int letter=0,i=-1,j=0,k=0,spaceflag=0,errflag=0,endoffuncflag=0,comandlen=0,freeflag=0;
	stringoffile *arrofstr1=NULL;
	char *rptr=NULL;
	stringoffile *rarrofstr=NULL;
	
	char print_pages[]="print pages";
	char print_range[]="print range";
	char set_tabwidth[]="set tabwidth";
	char set_wrap[]="set wrap";
	char insert_after[]="insert after";
	char edit_string[]="edit string";
	char insert_symbol[]="insert symbol";
	char replace_substring[]="replace substring";                          /*МАССИВЫ КОМАНД*/
	char delete_range[]="delete range";
	char delete_braces[]="delete braces";
	char exitminivim[]="exit";
	char openfilewosave[]="read";
	char openfilewsave[]="open";
	char filesave[]="write";
	char set_name[]="set name";
	char help[]="help";


	while (1)
	{
			if (exitflag==1)
				return;

			if ((endoffuncflag==1)&&(freeflag==0))
			{
				free(comand);
				comand=NULL;
			}

			spaceflag=0;
			errflag=0;
			letter=0;
			for (i=0;i<COMANDLEN+2;i++)
				comand_buf[i]=0;
			i=-1;
			j=0;
			endoffuncflag=0;
			comandreadflag=0;
			freeflag=0;

		printf(">Введите команду.\n");                                     /*ЧТЕНИЕ КОМАНДЫ*/
		printf(">");

		letter=(int)getchar();	
		while ((letter==' ')||(letter=='	'))
		{
			letter=(int)getchar();
		}
	
		while (letter!='\n')
		{
			if (((letter==' ')||(letter=='	'))&&(spaceflag==0))
			{
				i++;
				rptr=(char *)realloc(comand,(i+1)*sizeof(char));
				if (rptr==NULL)
				{
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return;
				}
				else
				{
					comand=rptr;
					rptr=NULL;
				}

				comand[i]=' ';
				spaceflag=1;
			}
			else
			if ((letter!=' ')&&(letter!='	'))
			{
				i++;
				rptr=(char *)realloc(comand,(i+1)*sizeof(char));
				if (rptr==NULL)
				{
					printf("===Недостаточно памяти.===\n");
					memoryerrflag=1;
					return ;
				}
				else
				{
					comand=rptr;
					rptr=NULL;
				}

				comand[i]=letter;
				spaceflag=0;
			}

			letter=(int)getchar();		
		}

		i++;
		rptr=(char *)realloc(comand,(i+1)*sizeof(char));
		if (rptr==NULL)
		{
			printf("===Недостаточно памяти.===\n");
			memoryerrflag=1;
			return;
		}
		else
		{
			comand=rptr;
			rptr=NULL;
		}
		comand[i]='\0';
		comandlen=i;
		i=0;

		for (j=0;j<2;j++)
		{
			if (endoffuncflag==1)
			{
				free(comand);
				comand=NULL;
				freeflag=1;
				break;
			}

			if (spaceflag==2)
			{
				free(comand);
				comand=NULL;
				freeflag=1;
				errflag=1;
			}

			if (j==1)
			{
				comandreadflag=1;
				if (spaceflag==1)
				{
					comand_buf[i]=' ';
					i++;
					if (i>COMANDLEN)
					{
						errflag=1;
						break;
					}
				}
			}

			if ((errflag!=1)||(spaceflag!=2))
			{	
				if (comand[0]=='\0')
				{
					comand_buf[0]='\0';
					spaceflag=2;
					break;
				}
				else
				while ((comand[i]!=' '))
				{
					comand_buf[i]=comand[i];
					i++;

					if (comand[i]=='\0')
					{
						comand_buf[i]='\0';
						spaceflag=2;
						break;
					}
					else
					if (comand[i]==' ')
					{
						comand_buf[i]='\0';
						spaceflag=1;
					}

					if (i>COMANDLEN)
					{
						errflag=1;
						break;
					}
				}
			}

			if (errflag==1)
			{
				printf("===Такой команды не существует.===\n");
				printf("===Введите команду заново.===\n");
				break;
			}
			else
			if (errflag!=1)
			{
				if (cmpstr(comand_buf,print_pages)==0)                     /*АНАЛИЗАТОР КОМАНД*/
				{
					comandrest=i;
					fprint_pages(arrofstr);

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,print_range)==0)
				{
					comandrest=i;
					fprint_range(arrofstr);

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/     
				}
				else
				if (cmpstr(comand_buf,set_tabwidth)==0)
				{
					comandrest=i;
					fset_tabwidth();

					if (settabwidthflag==-1)
					{
						settabwidthflag=8;
					}

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,set_wrap)==0)
				{
					comandrest=i;
					fset_wrap();

					if (setwrapflag==-1)
					{
						setwrapflag=1;
					}

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,insert_after)==0)
				{
					comandrest=i;
					arrofstr=finsert_after(arrofstr);

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,edit_string)==0)
				{
					comandrest=i;
					arrofstr=fedit_string(arrofstr);

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,insert_symbol)==0)
				{
					comandrest=i;
					arrofstr=finsert_symbol(arrofstr);

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,replace_substring)==0)
				{
					comandrest=i;
					arrofstr=freplace_substring(arrofstr);

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,delete_range)==0)
				{
					comandrest=i;
					arrofstr=fdelete_range(arrofstr);

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,delete_braces)==0)
				{
					comandrest=i;
					arrofstr=fdelete_braces(arrofstr);

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,exitminivim)==0)
				{
					comandrest=i;
					fexit(arrofstr);

					if (exitflag==1)
						break;

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,openfilewosave)==0)
				{
					comandrest=i;

					arrofstr1=freadfile(arrofstr);

					if ((arrofstr1!=NULL)||(((arrofstr1==NULL)&&(emptyfile==1))))
					{
						arrofstr=arrofstr1;
					}
					arrofstr1=NULL;

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,openfilewsave)==0)
				{
					comandrest=i;

					arrofstr1=fopenfile(arrofstr);

					if ((arrofstr1!=NULL)||(((arrofstr1==NULL)&&(emptyfile==1))))
					{
						arrofstr=arrofstr1;
					}
					arrofstr1=NULL;

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,filesave)==0)
				{
					comandrest=i;
					ffilesave(arrofstr);

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,set_name)==0)
				{
					comandrest=i;
					fset_name();

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if (cmpstr(comand_buf,help)==0)
				{
					comandrest=i;
					fhelp();

					if (memoryerrflag==1)
					{
						fexit(arrofstr);
						break;
					}

					endoffuncflag=1;                                       /*++++++++++++++++++++++++++++*/
				}
				else
				if ((comandreadflag==1)&&(endoffuncflag==0))
				{
					printf("===Такой команды не существует.===\n");
					printf("===Введите команду заново.===\n");
				}
			}
		}
	}
}

int cmpstr (char *str1, char *str2)
{
	int eos=0;


	while (str1[eos]!='\0')
	{
		if (str1[eos]!=str2[eos])
			return 1;
		else
		eos++;
	}
	if (str2[eos]=='\0')
		return 0;
}

stringoffile *file_toram (FILE *f)
{
	stringoffile *arrofstr=NULL,*arrofstr2=NULL;
	stringoffile str2;
	char *ptr=NULL;


	while (1)                                                              /*СОЗДАНИЕ МАССИВА СТРУКТУР*/
	{
		ptr=toram(f);
		
		if (endflag==1)
		{
			free(ptr);
			ptr=NULL;
			break;
		}
		else
		{
			str2.str=ptr;
			str2.stringlenght=charnum;
			ptr=NULL;

			countofstr++;

			arrofstr2=(stringoffile *)realloc(arrofstr,countofstr*sizeof(stringoffile));
			if (arrofstr2==NULL)
			{
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return arrofstr2;
			}
			else
			{
				arrofstr=arrofstr2;
				arrofstr2=NULL;
			}

			arrofstr[countofstr-1].str=str2.str;
			arrofstr[countofstr-1].stringlenght=str2.stringlenght;
		}
	}
	return arrofstr;
}

char *toram (FILE *f)
{
	charnum=0;
	int stepram=1;
	int letter=' '; 
	char *ptr=NULL,*rptr=NULL;


	endflag=0;
	ptr=(char *)malloc(STRLEN*sizeof(char));
	if (ptr==NULL)
	{
		printf("===Недостаточно памяти.===\n");
		memoryerrflag=1;
		return ptr;
	}
	
	while (letter!='\n')
	{
		letter=(char)fgetc(f); 

		if (feof(f))
		{
			endflag=1;
			break;
		}

		emptyfile=0;

		ptr[charnum]=(char)letter;
		charnum++;
		
		if (charnum>stepram*STRLEN-1)
		{
			stepram++;
			rptr=(char *)realloc(ptr,stepram*STRLEN*sizeof(char));
			if (rptr==NULL)
			{
				free(ptr);
				ptr=NULL;
				printf("===Недостаточно памяти.===\n");
				memoryerrflag=1;
				return rptr;
			}
			else
			{
				ptr=rptr;
				rptr=NULL;
			}
		}	
	}
	return ptr;
}