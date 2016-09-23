#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>  
#include <sys/wait.h>
#include <signal.h>


#define PASS		0x0		/*00000000*/
#define ADDSTR		0x1		/*00000001*/
#define ADDCHAR		0x2		/*00000010*/
#define ADDVAR		0x4		/*00000100*/
#define CHANGECHAR	0x8		/*00001000*/
#define NEWLINE		0x40	/*01000000*/

#define REWRITE 1
#define APPEND 1
#define LEN 50
#define VAR_SIZE 512

#define DIR_LEN 1024
#define MAX_HIST 101


typedef struct                                                            /*СТРУКТУРА ПРОГРАММЫ*/
{
    uid_t uid;
    pid_t pid;
    int prog_status;
	int number_of_arguments;
	char **arguments;
}shell_struct;

typedef struct                                                            /*СТРУКТУРА КОНВЕЙЕРА*/
{
	int background;          /*!&==0; &==1*/
    char **program;          /*вся строка*/
    char ***conv;            /*строка, разбитая по |*/
	int number_of_programs;  /*количество элементов конвейера*/
}job_struct;

job_struct *shell_jobs = NULL;
int count_of_shell_jobs = 0, bslashflag = 0;

pid_t pid;

char *current_dir = NULL, **in_command = NULL, ***history_arr = NULL; 
int cur_dir_len = 0, hist_size = 0, end_of_file = 0;

shell_struct shellprogram;

int preparing ();

void add_history(char **str);
void write_history();
void mcat(char *prog);
void mgrep (char **prog);

char **read_command ();
char *arg_substitution(char *buf);
int symbol_analyze (int letter, int *flag);

void build_jobs ();
void build_convs ();

char **delete_redirection_signs (char **prog);
int check_redirection_in_file (char **prog, char status);
int redirection ();
void execute (job_struct job);

void free_char2 (char **str);
void free_char3 (char ***str);
void free_jobs ();
void free_history();
void free_all ();


int main (int argc, char **argv)
{
    char shell_print[22] = "causelove*_*shell:~$ ";


    shellprogram.uid = getuid();
    shellprogram.pid = getpid();
    shellprogram.prog_status = 0;
	shellprogram.number_of_arguments = argc;
    shellprogram.arguments = argv;

    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    

    history_arr = (char ***) malloc(MAX_HIST*sizeof(char **));
    if (history_arr == NULL)
    {
    	fprintf(stderr, "Memory error\n");
    	free_all();
    	exit(-1);
    }

    history_arr[0] = NULL;

    current_dir = (char *) malloc(DIR_LEN*sizeof(char));
    if (current_dir == NULL)
    {
    	fprintf(stderr, "Memory error\n");
    	free_all();
    	exit(-1);
    }

    while (shell_print[cur_dir_len] != '\0')
    {
    	current_dir[cur_dir_len] = shell_print[cur_dir_len];
    	cur_dir_len++;
    }
    current_dir[cur_dir_len] = '\0';


    while(1)
    {
    	preparing();
    }
	
	return 0;
}

int preparing ()
{
	int i = 0, j = 0, k = 0, l = 0, t = 0, status = 0, subst_str = 0;
	pid_t pid;
	char *str = NULL;


	while ((pid = waitpid(0, NULL, WNOHANG | WUNTRACED)) > 0)
		printf("Process with ID (%d) stopped with status %d\n",pid, WEXITSTATUS(status));

	printf("%s", current_dir);

	in_command = read_command();

	if ((end_of_file == 1) && ((in_command == NULL)))
	{
		in_command = NULL;
		free_history();
		free(current_dir);
		exit(0);
	}

	if (in_command == NULL)
		return 0;

	while (in_command[i] != NULL)
	{
		if (in_command[i][0] == '!')
		{
			subst_str = atoi(in_command[i]+1);

			if ((subst_str > 0) && (subst_str <= hist_size))
			{
				j = 0;
				k = 0;

				while (in_command[j++] != NULL);
				while (history_arr[subst_str-1][k++] != NULL);

				in_command = (char **) realloc(in_command, (j+k-2)*sizeof(char *));
				if (in_command == NULL)
			    {
			    	fprintf(stderr, "Memory error\n");
			    	free_all();
			    	exit(-1);
			    }

				for (l = 0; l < (j-i-1); l++)
					in_command[(j+k-3)-l] = in_command[(j-1)-l];

				free(in_command[i]);

				j = 0;

				for (l = i; l < (i+k-1); l++)
				{
					t = 0;

					while (history_arr[subst_str-1][j][t++] != '\0');

					str = (char *) malloc(t*sizeof(char));
					if (str == NULL)
				    {
				    	fprintf(stderr, "Memory error\n");
				    	free_all();
				    	exit(-1);
				    }

					memcpy(str, history_arr[subst_str-1][j++], t);

					in_command[l] = str;
					str = NULL;
				}
			}
			else
			{
				fprintf(stderr, "history: %s: event not found\n",in_command[0]);
				free_char2(in_command);
				in_command = NULL;
				return 0;
			}			
		}

		i++;
	}

	build_jobs();
	build_convs();
	add_history(in_command);

	for (i = 0; i < count_of_shell_jobs; i++)
	{
		if ((shell_jobs[i].program[0] != NULL) && (strcmp(shell_jobs[i].program[0], "|") == 0))
		{
			printf("syntax error near '|'\n");
			break;
		}

		if ((shell_jobs[i].program[0] != NULL) && (strcmp(shell_jobs[i].program[0], "cd") == 0))
		{
			if (shell_jobs[i].program[1] != NULL)
			{
				if (strcmp(shell_jobs[i].program[1], "~") == 0)
				{
					if (chdir((getenv("HOME"))) != 0)
						printf("bash: cd: %s: No such file or directory\n", shell_jobs[i].program[1]);
				}
				else
				if (chdir(shell_jobs[i].program[1]) != 0)
					printf("bash: cd: %s: No such file or directory\n", shell_jobs[i].program[1]);
			}
			else
			{
				printf("cd: (null): No such file or directory\n");
			}

			continue;
		}

		if ((shell_jobs[i].program[0] != NULL) && (strcmp(shell_jobs[i].program[0], "exit") == 0))
		{
			in_command = NULL;
			free_all();
			free(current_dir);
			exit(0);
		}

		execute(shell_jobs[i]);

		if ((i == count_of_shell_jobs-1) && (end_of_file == 1))
		{
			in_command = NULL;
			free_all();
			free(current_dir);
			exit(0);
		}
	}

	in_command = NULL;
	free_jobs();

	return 0;
}

void mgrep (char **prog)
{
	int vflag = 0, letter = 0, i = 0;
	char *tmpstr = NULL, *str = NULL;
	FILE *fd = stdin;


	if (prog[1] == NULL)
	{
		fprintf(stderr, "mgrep: too few arguments\n");
		return;
	}

	if (strcmp(prog[1], "-v") == 0)
	{
		vflag = 1;
		
		if (prog[2] == NULL)
		{
			fprintf(stderr, "mgrep: too few arguments\n");
			return;
		}

		str = prog[2];

		if (prog[3] != NULL)
		{
			if ((fd = fopen(prog[3], "r")) == NULL)
			{
				perror("input");
				return;
			}
		}
	}
	else
	{
		str = prog[1];

		if (prog[2] != NULL)
		{
			if ((fd = fopen(prog[2], "r")) == NULL)
			{
				perror("input");
				return;
			}
		}
	}

	while (1)
	{
		tmpstr = NULL;

		i = 0;

		while (1)
		{
			letter = getc(fd);

			tmpstr = (char *) realloc(tmpstr, (++i)*sizeof(char));
			if (tmpstr == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }

			if ((letter == EOF) || (letter == '\n'))
			{
				tmpstr[i-1] = '\0';
				break;
			}
			else
				tmpstr[i-1] = letter;
		}

		if (((vflag == 0) && (strstr(tmpstr, str) != NULL)) || ((vflag == 1) && (strstr(tmpstr, str) == NULL)))
			printf("%s\n",tmpstr);

		if (tmpstr != NULL)
			free(tmpstr);

		if (letter == EOF)
			break;
	}

	str = NULL;

	if (fd != stdin)
		fclose(fd);

	return;
}

void mcat(char *prog)
{
	char letter = 0;
	FILE *fd = stdin;


	if (prog != NULL)
	{
		if ((fd = fopen(prog, "r")) == NULL)
		{
			fprintf(stderr, "cat: %s: no such file or directory\n",prog);
			return;
		}
	}

	while (1)
	{
		letter = getc(fd);

		if (letter == EOF)
			break;

		putchar(letter);
	}

	if (fd != stdin)
		fclose(fd);

	return;
}

void msed (char **prog)
{
	int flag = 0, i = 0, k = 0, letter = 0, len1 = 0, len2 = 0, j = 0, l = 0;
	char *str1 = NULL, *str2 = NULL, *tmpstr = NULL, *ptr =NULL;

	if ((prog[1] == NULL) || (prog[2] == NULL))
	{
		fprintf(stderr, "msed: too few arguments\n");
		return;
	}

	if (prog[3] != NULL)
	{
		fprintf(stderr, "msed: too many arguments\n");
		return;
	}

	if (prog[1][0] == '^')
		flag = 1;
	else
	if (prog[1][0] == '$')
		flag = 2;
	else
	{
		while (prog[1][len1++] != '\0');
		str1 = (char *) malloc(len1*sizeof(char));
		if (str1 == NULL)
	    {
	    	fprintf(stderr, "Memory error\n");
	    	free_all();
	    	exit(-1);
	    }
		memcpy(str1, prog[1], len1);
	}

	while (prog[2][len2++] != '\0');
	str2 = (char *) malloc(len2*sizeof(char));
	if (str2 == NULL)
    {
    	fprintf(stderr, "Memory error\n");
    	free_all();
    	exit(-1);
    }
	memcpy(str2, prog[2], len2);

	while (1)
	{
		tmpstr = NULL;

		i = 0;

		while (1)
		{
			letter = getchar();

			tmpstr = (char *) realloc(tmpstr, (++i)*sizeof(char));
			if (tmpstr == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }

			if ((letter == EOF) || (letter == '\n'))
			{
				tmpstr[i-1] = '\0';
				break;
			}
			else
				tmpstr[i-1] = letter;
		}

		if (flag == 1)
		{
			tmpstr = (char *) realloc(tmpstr, (i+len2-1)*sizeof(char));
			if (tmpstr == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }

			for (j = 0; j < i; j++)
				tmpstr[(i+len2-2)-j] = tmpstr[(i-1)-j];

			for (j = 0; j < len2-1; j++)
				tmpstr[j] = str2[j];

			tmpstr[i+len2-2] = '\0';
			printf("%s\n",tmpstr);
		}
		else
		if (flag == 2)
		{
			tmpstr = (char *) realloc(tmpstr, (i+len2-1)*sizeof(char));
			if (tmpstr == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }

			for (j = 0; j < len2-1; j++)
				tmpstr[(i-1)+j] = str2[j];

			tmpstr[i+len2-2] = '\0';
			printf("%s\n",tmpstr);
		}
		else
		{
			tmpstr[i-1] = '\0';
			ptr = tmpstr;
			l = 0;

			while ((strstr(ptr, str1)) != NULL)
			{
				j = 0;

				if (len1 < len2)
				{
					tmpstr = (char *) realloc(tmpstr, (i+len2-len1)*sizeof(char));
					if (tmpstr == NULL)
				    {
				    	fprintf(stderr, "Memory error\n");
				    	free_all();
				    	exit(-1);
				    }

					if (l != 0)
						ptr = &tmpstr[l];
					else
						ptr = &tmpstr[0];

					ptr = strstr(ptr, str1);

					while (ptr[j++] != '\0');

					for (l = 0; l < j; l++)
						tmpstr[(i+len2-len1-1)-l] = tmpstr[(i-1)-l];

					for (l = 0; l < len2-1; l++)
						ptr[l] = str2[l];

					i = i+len2-len1;

					if (ptr[l] != '\0')
					{
						ptr = &ptr[l];

						l = 0;
						while (ptr[l++] != '\0');
						l = i-l;
					}
					else
						break;
				}
				else
				if (len1 > len2)
				{
					ptr = strstr(ptr, str1);

					for (l = 0; l < len2-1; l++)
						ptr[l] = str2[l];

					j = l;
					k = j+len1-len2;
					while(ptr[k] != '\0')
					{
						ptr[j] = ptr[k];
						k++;
						j++;
					}

					ptr[j] = '\0';

					if (ptr[l] != '\0')
						ptr = &ptr[l];
					else
						break;
				}
				else
				if (len1 == len2)
				{
					ptr = strstr(ptr, str1);

					for (l = 0; l < len2-1; l++)
						ptr[l] = str2[l];

					if (ptr[l] != '\0')
						ptr = &ptr[l];
					else
						break;
				}
			}

			ptr = NULL;

			printf("%s\n",tmpstr);
		}

		free(tmpstr);
		tmpstr = NULL;

		if (letter == EOF)
			break;
	}

	if (str1 != NULL)
		free(str1);
	free(str2);

	str1 = NULL;
	str2 = NULL;

	return;
}

void add_history(char **str)
{
	int i = 0;


	if (hist_size <= MAX_HIST-2)
	{
		history_arr[hist_size++] = str;
		history_arr[hist_size] = NULL;
	}
	else
	{	
		free(history_arr[0]);
		history_arr[0] = NULL;

		for (i = 1; i < MAX_HIST-1; i++)
			history_arr[i-1] = history_arr[i];

		history_arr[i-1] = str;
		history_arr[i] = NULL;
	}

	return;
}

void write_history()
{
	int i = 0, j = 0;


	for (i = 0; i < hist_size; i++)
	{
		j = 0;
		printf("  !%d  ",i+1);
		while (history_arr[i][j] != NULL)
			printf("%s ",history_arr[i][j++]);
		printf("\n");
	}

	return;
}

char **delete_redirection_signs (char **prog)
{
	int i = 0, arg_count = 0, new_size = 0;
	char **tmpprog = NULL;


	for (arg_count = 0; prog[arg_count] != NULL; arg_count++);

	while (i != arg_count)
	{
		if ((strcmp(prog[i], "<") == 0) || (strcmp(prog[i], ">") == 0) || (strcmp(prog[i], ">>") == 0))
		{
			i++;
		}
		else
		{
			tmpprog = (char **) realloc(tmpprog, (++new_size+1)*sizeof(char *));
			if (tmpprog == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }
			tmpprog[new_size-1] = prog[i];
			tmpprog[new_size] = NULL;
		}

		i++;
	}

	return tmpprog;
}

int check_redirection_in_file (char **prog, char status)
{
	int fd = 0, i = 0;


	while (prog[i] != NULL)
	{
		if ((status == 'r') && (strcmp(prog[i], "<") == 0))
		{
			if (prog[i+1] == NULL)
			{
				fprintf(stderr, "syntax error near '<'\n");
				return -1;
			}
			else
			if ((fd = open(prog[i+1], O_RDONLY)) == -1)
			{
				perror("input");
				return -1;
			}
			else
				return fd;
		}
		else
		if ((status == 'w') && (strcmp(prog[i], ">") == 0))
		{
			if (prog[i+1] == NULL)
			{
				fprintf(stderr, "syntax error near '>'\n");
				return -1;
			}
			else
			if ((fd = open(prog[i+1], O_TRUNC | O_WRONLY | O_CREAT, 0666)) == -1)
			{
				perror("output");
				return -1;
			}
			else
				return fd;
		}
		else
		if ((status == 'w') && (strcmp(prog[i], ">>") == 0))
		{
			if (prog[i+1] == NULL)
			{
				fprintf(stderr, "syntax error near '>>'\n");
				return -1;
			}
			else
			if ((fd = open(prog[i+1], O_APPEND | O_WRONLY | O_CREAT, 0666)) == -1)
			{
				perror("output");
				return -1;
			}
			else
				return fd;
		}

		i++;
	}

	return 0;
}

int redirection (char **prog)
{
	int fd = 0;


	fd = check_redirection_in_file(prog, 'r');

	if (fd == -1)
	{
		return -1;
	}

	if (fd != 0)
	{
		dup2(fd, 0);
		close(fd);
	}

	fd = check_redirection_in_file(prog, 'w');

	if (fd == -1)
	{
		return -1;
	}

	if (fd != 0)
	{
		dup2(fd, 1);
		close(fd);
	}

	return 0;
}

/*void execute (job_struct job)
{
	int lfd[2], rfd[2], fd = 0, i = 0, status = 0; 

	switch ((pid = fork()))
	{
		case -1:
		{
			perror("fork");
			return;
		}

		case 0:
		{
			setpgid(getpid(), getpid());

			for (i = 0; i < job.number_of_programs; i++)
			{
				if (job.number_of_programs != 1)
					pipe(rfd);

				switch (fork())
				{
					case -1:
					{
						perror("fork");
						return;
					}

					case 0:
					{
						char *str = NULL, **tmpprog = NULL;

						if (job.number_of_programs == 1)
						{
							if (job.background == 1)
								printf("Process with ID (%d) launched\n",getpid());

							fd = redirection(job.conv[i]);

							if (fd == -1)  
							{
								in_command = NULL;
								free_all();
								free(current_dir);
								exit(-1);
							}
						}
						else
						{
							if (job.background == 1)
								printf("Process with ID (%d) launched\n",getpid());

							if (i == 0)
							{
								dup2(rfd[1], 1);
								close(rfd[0]);
								close(rfd[1]);

								fd = redirection(job.conv[i]);

								if (fd == -1)  
								{
									in_command = NULL;
									free_all();
									free(current_dir);
									exit(-1);
								}
							}
							else
							if (i != job.number_of_programs-1)
							{
								dup2(lfd[0], 0);
								dup2(rfd[1], 1);

								close(rfd[0]);
								close(rfd[1]);

								close(lfd[0]);
								close(lfd[1]);
							}
							else
							if (i == job.number_of_programs-1)
							{
								dup2(lfd[0], 0);

								fd = redirection(job.conv[i]);

								if (fd == -1)  
								{
									in_command = NULL;
									free_all();
									free(current_dir);
									exit(-1);
								}

								close(rfd[0]);
								close(rfd[1]);

								close(lfd[0]);
								close(lfd[1]);
							}
						}

						tmpprog = delete_redirection_signs(job.conv[i]);

						signal(SIGINT, SIG_DFL);
						signal(SIGTSTP, SIG_DFL);

						if ((tmpprog[0] != NULL) && (strcmp(tmpprog[0], "history") == 0))
						{
							write_history();
							in_command = NULL;
							free(tmpprog);
							tmpprog = NULL;
							free_all();
							free(current_dir);
							exit(0);
						}
						else
						if ((tmpprog[0] != NULL) && (strcmp(tmpprog[0], "pwd") == 0))
						{
							printf("%s\n", str = getcwd(NULL, 0));
							free(str);
							str = NULL;
							in_command = NULL;
							free(tmpprog);
							tmpprog = NULL;
							free_all();
							free(current_dir);
							exit(0);
						}
						else
						if ((tmpprog[0] != NULL) && (strcmp(tmpprog[0], "mcat") == 0))
						{
							mcat(tmpprog[1]);
							in_command = NULL;
							free(tmpprog);
							tmpprog = NULL;
							free_all();
							free(current_dir);
							exit(0);
						}
						else
						if ((tmpprog[0] != NULL) && (strcmp(tmpprog[0], "mgrep") == 0))
						{
							mgrep(tmpprog);
							in_command = NULL;
							free(tmpprog);
							tmpprog = NULL;
							free_all();
							free(current_dir);
							exit(0);
						}
						else
						{
							execvp(tmpprog[0], tmpprog);
							perror("exec");

							in_command = NULL;
							free(tmpprog);
							tmpprog = NULL;
							free_all();
							free(current_dir);
							exit(-1);
						}
					}

					default:
					{
						if (i > 0)
						{
							close(lfd[0]);
							close(lfd[1]);
						}

						if (job.number_of_programs != 1)
						{
							lfd[0] = rfd[0];
							lfd[1] = rfd[1];
						}
					}
				}
			}

		}

		default:
		{
			if (job.background == 0)
			{
				waitpid(pid, &shellprogram.prog_status, WUNTRACED);
				tcsetpgrp(0, pid);
			}
		}
	}

	return;
}*/

void execute (job_struct job)
{
	int lfd[2], rfd[2], fd = 0, i = 0; 


	for (i = 0; i < job.number_of_programs; i++)
	{
		if (job.number_of_programs != 1)
			pipe(rfd);

		switch ((pid = fork()))
		{
			case -1:
			{
				perror("fork");
				return;
			}

			case 0:
			{
				char *str = NULL, **tmpprog = NULL;

				if (job.number_of_programs == 1)
				{
					if (job.background == 1)
						printf("Process with ID (%d) launched\n",getpid());

					fd = redirection(job.conv[i]);

					if (fd == -1)  
					{
						in_command = NULL;
						free_all();
						free(current_dir);
						exit(-1);
					}
				}
				else
				{
					if (job.background == 1)
						printf("Process with ID (%d) launched\n",getpid());

					if (i == 0)
					{
						dup2(rfd[1], 1);
						close(rfd[0]);
						close(rfd[1]);

						fd = redirection(job.conv[i]);

						if (fd == -1)  
						{
							in_command = NULL;
							free_all();
							free(current_dir);
							exit(-1);
						}
					}
					else
					if (i != job.number_of_programs-1)
					{
						dup2(lfd[0], 0);
						dup2(rfd[1], 1);

						close(rfd[0]);
						close(rfd[1]);

						close(lfd[0]);
						close(lfd[1]);
					}
					else
					if (i == job.number_of_programs-1)
					{
						dup2(lfd[0], 0);

						fd = redirection(job.conv[i]);

						if (fd == -1)  
						{
							in_command = NULL;
							free_all();
							free(current_dir);
							exit(-1);
						}

						close(rfd[0]);
						close(rfd[1]);

						close(lfd[0]);
						close(lfd[1]);
					}
				}

				tmpprog = delete_redirection_signs(job.conv[i]);

				signal(SIGINT, SIG_DFL);
				signal(SIGTSTP, SIG_DFL);

				if ((tmpprog[0] != NULL) && (strcmp(tmpprog[0], "history") == 0))
				{
					write_history();
					in_command = NULL;
					free(tmpprog);
					tmpprog = NULL;
					free_all();
					free(current_dir);
					exit(0);
				}
				else
				if ((tmpprog[0] != NULL) && (strcmp(tmpprog[0], "pwd") == 0))
				{
					printf("%s\n", str = getcwd(NULL, 0));
					free(str);
					str = NULL;
					in_command = NULL;
					free(tmpprog);
					tmpprog = NULL;
					free_all();
					free(current_dir);
					exit(0);
				}
				else
				if ((tmpprog[0] != NULL) && (strcmp(tmpprog[0], "mcat") == 0))
				{
					mcat(tmpprog[1]);
					in_command = NULL;
					free(tmpprog);
					tmpprog = NULL;
					free_all();
					free(current_dir);
					exit(0);
				}
				else
				if ((tmpprog[0] != NULL) && (strcmp(tmpprog[0], "mgrep") == 0))
				{
					mgrep(tmpprog);
					in_command = NULL;
					free(tmpprog);
					tmpprog = NULL;
					free_all();
					free(current_dir);
					exit(0);
				}
				else
				if ((tmpprog[0] != NULL) && (strcmp(tmpprog[0], "msed") == 0))
				{
					msed(tmpprog);
					in_command = NULL;
					free(tmpprog);
					tmpprog = NULL;
					free_all();
					free(current_dir);
					exit(0);
				}
				else
				{
					execvp(tmpprog[0], tmpprog);
					perror("exec");

					in_command = NULL;
					free(tmpprog);
					tmpprog = NULL;
					free_all();
					free(current_dir);
					exit(-1);
				}
			}

			default:
			{
				if (i > 0)
				{
					close(lfd[0]);
					close(lfd[1]);
				}

				if (job.number_of_programs != 1)
				{
					lfd[0] = rfd[0];
					lfd[1] = rfd[1];
				}

				if (job.background == 0)
					wait(&shellprogram.prog_status);

				/*if ((job.background == 1) && (WEXITSTATUS(&shellprogram.prog_status) != 0))
					break;*/
			}
		}
	}

	return;
}

void build_jobs ()
{
	int i = 0, count_of_args = 0;
	char **new_job = NULL;


	count_of_shell_jobs = 0;

	do
	{
		if (((count_of_args != 0) && (in_command[i] == NULL)) || ((in_command[i] != NULL) && (strcmp(in_command[i], ";") == 0)))
		{
			shell_jobs = (job_struct *) realloc(shell_jobs, ++count_of_shell_jobs*sizeof(job_struct));
			if (shell_jobs == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }
			shell_jobs[count_of_shell_jobs-1].background = 0;
			shell_jobs[count_of_shell_jobs-1].program = new_job;
			shell_jobs[count_of_shell_jobs-1].conv = NULL;

			new_job = NULL;
			new_job = (char **) malloc(sizeof(char *));
			if (new_job == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }
			*new_job = NULL;
			count_of_args = 0;
		}
		else
		if ((in_command[i] != NULL) && (strcmp(in_command[i], "&") == 0))
		{
			shell_jobs = (job_struct *) realloc(shell_jobs, ++count_of_shell_jobs*sizeof(job_struct));
			if (shell_jobs == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }
			shell_jobs[count_of_shell_jobs-1].background = 1;
			shell_jobs[count_of_shell_jobs-1].program = new_job;
			shell_jobs[count_of_shell_jobs-1].conv = NULL;

			new_job = NULL;
			new_job = (char **) malloc(sizeof(char *));
			if (new_job == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }
			*new_job = NULL;
			count_of_args = 0;
		}
		else
		{
			new_job = (char **) realloc(new_job, (++count_of_args+1)*sizeof(char *));
			if (new_job == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }
			new_job[count_of_args-1] = in_command[i];
			new_job[count_of_args] = NULL;
		}

	} while (in_command[i++] != NULL);

	free(new_job);

	return;
}

void build_convs ()
{
	int i = 0, j = 0, count_of_args = 0;
	char **new_prog = NULL;


	for (i = 0; i < count_of_shell_jobs; i++)
	{	
		j = 0;
		count_of_args = 0;
		shell_jobs[i].number_of_programs = 0;

		do
		{
			if (((count_of_args != 0) && (shell_jobs[i].program[j] == NULL)) || ((shell_jobs[i].program[j] != NULL) && (strcmp(shell_jobs[i].program[j], "|") == 0)))
			{
				shell_jobs[i].conv = (char ***) realloc(shell_jobs[i].conv, (++shell_jobs[i].number_of_programs+1)*sizeof(char **));
				if (shell_jobs[i].conv == NULL)
			    {
			    	fprintf(stderr, "Memory error\n");
			    	free_all();
			    	exit(-1);
			    }
				shell_jobs[i].conv[shell_jobs[i].number_of_programs-1] = new_prog;
				shell_jobs[i].conv[shell_jobs[i].number_of_programs] = NULL;

				new_prog = NULL;
				count_of_args = 0;
			}
			else
			{
				new_prog = (char **) realloc(new_prog, (++count_of_args+1)*sizeof(char *));
				if (new_prog == NULL)
			    {
			    	fprintf(stderr, "Memory error\n");
			    	free_all();
			    	exit(-1);
			    }
				new_prog[count_of_args-1] = shell_jobs[i].program[j];
				new_prog[count_of_args] = NULL;
			}

		} while (shell_jobs[i].program[j++] != NULL);	
	}

	return;
}

void free_char2 (char **str)
{
	int i = 0;


	if (str != NULL)
	{
		while (str[i] != NULL)
		{
			free(str[i]);
			str[i++] = NULL;
		}

		free(str);
		str = NULL;
	}

	return;
}

void free_char3 (char ***str)
{
	int i = 0;


	if (str != NULL)
	{
		while (str[i] != NULL)
		{
			free(str[i]);
			str[i++] = NULL;
		}

		free(str);
		str = NULL;
	}

	return;
}

void free_jobs ()
{
	int i = 0;


	for (i = 0; i < count_of_shell_jobs; i++)
	{
		free(shell_jobs[i].program);
		shell_jobs[i].program = NULL;
		free_char3(shell_jobs[i].conv);
	}

	if (shell_jobs != NULL)
		free(shell_jobs);

	shell_jobs = NULL;

	return;
}

void free_history()
{
	int i = 0;


	for (i = 0; i < hist_size; i++)
		free_char2(history_arr[i]);

	free(history_arr);
	history_arr = NULL;

	return;
}

void free_all ()
{
	free_history();

	if (count_of_shell_jobs != 0)
		free_jobs();

	return;
}

char *arg_substitution(char *buf)
{
	int num = buf[1]-'0';


	if ((strcmp(buf, "$USER") == 0) || (strcmp(buf, "${USER}") == 0))
		strcpy(buf, getenv("USER"));

	if ((strcmp(buf, "$HOME") == 0) || (strcmp(buf, "${HOME}") == 0))
		strcpy(buf, getenv("HOME"));

	if ((strcmp(buf, "$PWD") == 0) || (strcmp(buf, "${PWD}") == 0))
		strcpy(buf, getenv("PWD"));

	if ((strcmp(buf, "$SHELL") == 0) || (strcmp(buf, "${SHELL}") == 0))
		strcpy(buf, getenv("SHELL"));

	if ((strcmp(buf, "$PID") == 0) || (strcmp(buf, "${PID}") == 0))
		sprintf(buf, "%d", shellprogram.pid);

	if ((strcmp(buf, "$UID") == 0) || (strcmp(buf, "${UID}") == 0))
		sprintf(buf, "%d", shellprogram.uid);

	if (strcmp(buf, "$#") == 0)
		sprintf(buf, "%d", shellprogram.number_of_arguments);

	if (strcmp(buf, "$?") == 0)
		sprintf(buf, "%d", shellprogram.prog_status);

	if ((num >= 0) && (num <= 9) && (num < shellprogram.number_of_arguments))
		strcpy(buf, shellprogram.arguments[num]);

	return buf;
}

char **read_command ()
{
	char **prog = NULL;
	char *str = NULL, *tempstr = NULL, *var_arr = (char *) calloc(VAR_SIZE, 1*sizeof(char));
	int letter = 0, count_of_prog = 0, str_len = 0, count_of_char = 0, var_len = 0, flag1 = 0, flag2 = 0;


	while (!(flag2 & NEWLINE))
	{
		flag2 = symbol_analyze(letter = getchar(), &flag1);

		if ((count_of_char != 0) && (strcmp(str, "|") == 0) && (letter == '\n'))
			flag2 = ADDSTR;

		if ((count_of_char != 0) && (letter == '>') && (strcmp(str, ">") == 0))
		{
			flag2 = ADDCHAR;
		}

		if (flag1 == 5)
		{
			var_arr[var_len++] = letter;
			
			if (letter == '}')
				flag1 = 1;
			else
				continue;
		}

		if (flag2 & ADDVAR)
		{
			str_len += VAR_SIZE;
			str = (char *) realloc(str, str_len*sizeof(char));
			if (str == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }
			strcpy(str+count_of_char, arg_substitution(var_arr));
			count_of_char = strlen(str);
			free(var_arr);
			var_arr = (char *) calloc(VAR_SIZE, 1*sizeof(char));
			if (var_arr == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }
			var_len = 0;
		}


		if ((flag2 & ADDSTR) && (str != NULL))
		{
			prog = (char **) realloc(prog, ++count_of_prog*sizeof(char *));
			if (prog == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }

			tempstr = (char *) malloc((count_of_char+1)*sizeof(char));
			if (tempstr == NULL)
		    {
		    	fprintf(stderr, "Memory error\n");
		    	free_all();
		    	exit(-1);
		    }
			memcpy(tempstr, str, count_of_char+1);
			prog[count_of_prog-1] = tempstr;

			count_of_char = 0;
			str_len = 0;
			free(str);
			str = NULL;
			tempstr = NULL;
		}

		if (flag2 & ADDCHAR)
		{
			if (++count_of_char >= str_len)
			{
				str_len += LEN; 
				str = (char *) realloc(str, str_len*sizeof(char));
				if (str == NULL)
			    {
			    	fprintf(stderr, "Memory error\n");
			    	free_all();
			    	exit(-1);
			    }
			}

			str[count_of_char-1] = letter;
			str[count_of_char] = '\0';
		}

		if (flag2 & CHANGECHAR)
			str[count_of_char-1] = letter;
	}

	free(var_arr);
	var_arr = NULL;

	if (letter == '#')
		while (getchar() != '\n');

	if (prog == NULL)
	{
		free(str);
		return NULL;
	}

	prog = (char **) realloc(prog, ++count_of_prog*sizeof(char *));
	if (prog == NULL)
    {
    	fprintf(stderr, "Memory error\n");
    	free_all();
    	exit(-1);
    }
	prog[count_of_prog-1] = NULL;


	if (str != NULL)
		free(str);

	return prog;
}

int symbol_analyze (int letter, int *flag)
{
	/*
	FLAGS
	0 == char
	1 == "
	2 == '
	3 == &; |; <; >; 
	4 == \ 
	5 == $
	*/

    switch (letter)
    {
        case '"':   if (*flag == 0) { *flag = 1; return PASS; }
                    if (*flag == 1) { *flag = 0; return PASS; }
                    if (*flag == 2) {            return ADDCHAR; }
                    if (*flag == 3) { *flag = 1; return ADDSTR | ADDCHAR; }
                    if (*flag == 4) { *flag = bslashflag; return CHANGECHAR; }
                    if (*flag == 5) { *flag = 0; return ADDVAR | ADDSTR; }

        case '\'':  if (*flag == 0) { *flag = 2; return PASS; }
                    if (*flag  == 1) {             return ADDCHAR;}
                    if (*flag == 2) { *flag = 0; return PASS; }
                    if (*flag == 3) { *flag = 2; return ADDSTR | ADDCHAR; }
                    if ((*flag == 4) && (bslashflag == 0)) { *flag = bslashflag; return CHANGECHAR; }
                    if ((*flag == 4) && (bslashflag == 1)) { *flag = bslashflag; return ADDCHAR; }
                    if ((*flag == 4) && (bslashflag == 2)) { *flag = 0;        return PASS; }
                    if ((*flag == 4) && (bslashflag  > 2)) { *flag = 0;        return CHANGECHAR; }
                    if (*flag == 5) { *flag = 1; return ADDVAR | ADDCHAR; }

        case '#':   if (*flag == 0) { return ADDSTR | NEWLINE; }
        			if (*flag == 1)	{ return ADDCHAR; }
        			if (*flag == 2)	{ return ADDCHAR; }
        			if (*flag == 3) { return ADDSTR | NEWLINE; }
        			if ((*flag == 4) && (bslashflag == 0)) { *flag = bslashflag; return CHANGECHAR; }
                    if ((*flag == 4) && (bslashflag == 1)) { *flag = bslashflag; return ADDCHAR; }
                    if ((*flag == 4) && (bslashflag == 2)) { *flag = bslashflag; return ADDCHAR; }
                    if ((*flag == 4) && (bslashflag  > 2)) { *flag = 0;        return CHANGECHAR; }
                    if (*flag == 5) { return PASS; }

        case ' ':   if (*flag == 0) { return ADDSTR; }
                    if (*flag == 1) { return ADDCHAR; }
                    if (*flag == 2) { return ADDCHAR; }
                    if (*flag == 3) { return ADDSTR; }
                    if (*flag == 4) { *flag = bslashflag; return ADDCHAR; }
                    if (*flag == 5) { *flag = 1;        return ADDVAR | ADDCHAR; }          

        case '\\':  if (*flag == 0) { *flag = 4; bslashflag = 0; return ADDCHAR; }
        			if (*flag == 1)	{ *flag = 4; bslashflag = 1; return ADDCHAR; }
        			if (*flag == 2)	{ *flag = 4; bslashflag = 2; return ADDCHAR; }  
        			if (*flag == 3)	{ *flag = 4; bslashflag = 3; return ADDSTR; }
        			if ((*flag == 4) && (bslashflag == 0)) { *flag = bslashflag; return CHANGECHAR; }
                    if ((*flag == 4) && (bslashflag == 1)) { *flag = bslashflag; return CHANGECHAR; }
                    if ((*flag == 4) && (bslashflag >= 2)) { *flag = bslashflag; return ADDCHAR; }
                    if (*flag == 5) { return PASS; }

        case '$':   if (*flag == 0) { return ADDCHAR; }
                    if (*flag == 1) { *flag = 5; return PASS; }
                    if (*flag == 2) { return ADDCHAR; }
                    if (*flag == 3) { *flag = 0; return ADDSTR | ADDCHAR; }
                    if ((*flag == 4) && (bslashflag == 0)) { *flag = bslashflag; return CHANGECHAR; }
                    if ((*flag == 4) && (bslashflag == 1)) { *flag = bslashflag; return CHANGECHAR; }
                    if ((*flag == 4) && (bslashflag >= 2)) { *flag = bslashflag; return ADDCHAR; }
                    if (*flag == 5) { return ADDVAR; }

        case '{':   if (*flag == 5) { return PASS; }           
        case '}':   if (*flag == 5) { *flag = 5; return ADDVAR; }

        case ';':
        case '&':
        case '|':
        case '<':  
        case '>':   if (*flag == 0) { *flag = 3; return ADDSTR | ADDCHAR; }
                    if (*flag == 1) { return ADDCHAR; }
                    if (*flag == 2) { return ADDCHAR; }
                    if (*flag == 3) { return ADDSTR | ADDCHAR; }
                    if ((*flag == 4) && (bslashflag == 0)) { *flag = bslashflag; return CHANGECHAR; }
                    if ((*flag == 4) && (bslashflag >= 1)) { *flag = bslashflag; return ADDCHAR; }
                    if (*flag == 5) { *flag = 1; return ADDVAR | ADDCHAR; }

        case ':':   if (*flag == 3) { *flag = 0; return ADDSTR | ADDCHAR; }  
                    if ((*flag == 4) && (bslashflag == 0)) { *flag = bslashflag; return CHANGECHAR; }
                    if ((*flag == 4) && (bslashflag == 1)) { *flag = bslashflag; return ADDCHAR; }
                    if ((*flag == 4) && (bslashflag == 2)) { *flag = bslashflag; return ADDCHAR; }
                    if ((*flag == 4) && (bslashflag  > 2)) { *flag = 0;        return CHANGECHAR; }
                    if (*flag == 5) {*flag = 1; return ADDVAR | ADDCHAR; }

        case '\n':  if (*flag == 0) { return ADDSTR | NEWLINE; }
                    if (*flag == 1) { return ADDCHAR; }
                    if (*flag == 2) { return ADDCHAR; }
                    if (*flag == 3) { return ADDSTR | NEWLINE; }
                    if (*flag == 4) { *flag = bslashflag; return PASS; }
                    if (*flag == 5) { return ADDCHAR; } 

        case EOF:   { end_of_file = 1; return ADDSTR | NEWLINE; } 
        
        default:    if (*flag == 3) { *flag = 0; return ADDSTR | ADDCHAR; }  
        			if ((*flag == 4) && (bslashflag == 0)) { *flag = bslashflag; return CHANGECHAR; }
                    if ((*flag == 4) && (bslashflag == 1)) { *flag = bslashflag; return ADDCHAR; }
                    if ((*flag == 4) && (bslashflag == 2)) { *flag = bslashflag; return ADDCHAR; }
                    if ((*flag == 4) && (bslashflag  > 2)) { *flag = 0;        return CHANGECHAR; }
                    else { return ADDCHAR; }
    }
}