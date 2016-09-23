#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define STRLEN 256

int filesort(FILE *f,int *numofstr,FILE *fo);
char *toram (FILE *f);
char *cmpstr(char *str1, char *str2);
void table (int *charmas, long width);


int charnum=0,strsum=0,flag=0;


int main(int argc, char const *argv[])
{
	FILE *infile;
	FILE *outfile;
	int i=0,k=0,dgt=0,charmas[STRLEN];
	char *mas=NULL;
	int letter=' ';
	long width=0;


	infile=fopen(argv[1],"rb");
	if (infile==NULL)
	{
		fprintf(stderr,"===Имя файла не существует.===\n");
		return 1;
	}

	outfile=fopen(argv[2],"wb+");
	if (outfile==NULL)
	{
		fprintf(stderr,"===Нет места на диске.===\n");
		return 1;
	}

	for (i=0;i<STRLEN;i++)
		charmas[i]=0;

	while (!feof(infile))
	{
		letter=(char)fgetc(infile); 
		if (!feof(infile))
		charmas[(int)abs(letter)]++;
		if ((letter=='\n')||(feof(infile)))
			strsum++;
	}

	int numofstr[strsum];

	for (i=0;i<strsum;i++)
		numofstr[i]=i+1;

	rewind(infile);

	if (filesort(infile,numofstr,outfile)==1)
	{
		fprintf(stderr,"===Недостаточно памяти.===\n");
		return 1;
	}

	if (argv[3]==NULL)
	{
		fprintf(stderr,"===Не задана ширина экрана.===\n");
		return 1;
	}
	width=atoi(argv[3]);
	table(charmas,width);

	fclose(infile);
	fclose(outfile);

	return 0;
}

void table (int *charmas, long width)
{
	int i=0,j=0,max=0;
	int total=0;


	max=charmas[0];

	if (width>63)
	{
		width=63;
		printf("===Больше, чем ширина экрана.Изменено на изначальную ширину.===\n");
	}

	for (i=1;i<STRLEN;i++)
		if (charmas[i]>max)
			max=charmas[i];

	for (i=0;i<STRLEN;i++)
	{
		if (charmas[i]!=0)
		{
			if (isprint(i)!=0)
			{
				printf("%-4c|%-10i|",i,charmas[i]);

				for (j=0;j<width*charmas[i]/max;j++)
					printf("*");

				for (;j<width;j++)
					printf(" ");

				printf("|\n");
			}
			else
			{
				printf("#%03i|%-10i|",i,charmas[i]);

				for (j=0;j<width*charmas[i]/max;j++)
					printf("*");

				for (;j<width;j++)
					printf(" ");

				printf("|\n");
			}
		}
		total=total+charmas[i];
	}
	printf("===Всего символов=== %i\n",total);
}

int filesort(FILE *f,int *numofstr,FILE *fo)
{
	char *ptr1=NULL,*ptr2=NULL, *last=NULL;
	int i,j,k=0,dgt,charnum1=0,charnum2=0,lastcharnum=0,lastflag=0,flag1=0,flag2=0;


	for (i=0;i<strsum-1;i++)
	{
		rewind(f);
		ptr1=toram(f);
		if (ptr1==NULL)
			return 1;

		charnum1=charnum;
		flag1=flag;
		for (k=1;k<numofstr[i];k++)
		{
			free(ptr1);
			ptr1=toram(f);
			if (ptr1==NULL)
				return 1;

			charnum1=charnum;
			flag1=flag;
		}

		for(j=i+1;j<strsum;j++)
		{
			rewind(f);
			ptr2=toram(f);
			if (ptr2==NULL)
				return 1;

			charnum2=charnum;
			flag2=flag;
			for (k=1;k<numofstr[j];k++)
			{
				free(ptr2);
				ptr2=toram(f);
				if (ptr2==NULL)
					return 1;

				charnum2=charnum;
				flag2=flag;
			}

			if (cmpstr(ptr1,ptr2)==ptr2)
			{
				k=numofstr[i];
				numofstr[i]=numofstr[j];
				numofstr[j]=k;

				if(i==strsum-2)
				{
					last=ptr1;
					lastcharnum=charnum1;
					lastflag=flag1;
					ptr1=NULL;
				}

				charnum1=charnum2;
				flag1=flag2;

				if (i!=strsum-2)
				free(ptr1);
				ptr1=ptr2;
				ptr2=NULL;
			}
			else
			{
				if(i==strsum-2)
				{
					last=ptr2;
					ptr2=NULL;
					lastflag=flag2;
					lastcharnum=charnum2;
				}
			}
			free(ptr2);
		}

		if (flag1==1)
			for (dgt=0;dgt<charnum1-1;dgt++)
				fputc((int)ptr1[dgt],fo); 
		else
			for (dgt=0;dgt<charnum1;dgt++)
				fputc((int)ptr1[dgt],fo); 

		if ((i==strsum-2)&&(lastflag==1))
			for (dgt=0;dgt<lastcharnum-1;dgt++)
				fputc((int)last[dgt],fo); 
		else
		if ((i==strsum-2)&&(lastflag==0))
			for (dgt=0;dgt<lastcharnum;dgt++)
				fputc((int)last[dgt],fo);

		free(last);
		free(ptr1);
	}
	return 0;
}

char *toram (FILE *f)
{
	charnum=0;
	int stepram=1;
	int letter=' '; 
	char *ptr=NULL,*rptr=NULL;


	flag=0;
	ptr=(char *)malloc(2*STRLEN*sizeof(char));
	if (ptr==NULL)
	{
		return ptr;
	}
	
	while (letter!='\n')
	{
		letter=(char)fgetc(f); 

		if ((feof(f))||(letter=='\n'))
		{
			ptr[charnum]='\n';

			if (feof(f))
			flag=1;

			charnum++;
			break;
		}

		ptr[charnum]=(char)letter;
		charnum++;
		
		if (charnum>stepram*STRLEN)
		{
			stepram++;
			rptr=(char *)realloc(ptr,2*stepram*STRLEN*sizeof(char));
			if (rptr==NULL)
			{
				free(rptr);
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

char *cmpstr(char *str1, char *str2)
{
	int eos=0;


	while (str1[eos]!='\n')
	{
		if ((str2[eos]=='\n')||(str1[eos]>str2[eos]))
			return str2;
		else
		if (str2[eos]>str1[eos])
			return str1;
		eos++;
	}
	return str1;
}