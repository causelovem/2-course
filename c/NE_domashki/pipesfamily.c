#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

pid_t pid_f, pid_s, pid_g1, pid_g2, pid_s2, pid_g21, pid_g22;

void handler(int sig) 
{
	/*if(getpid() == pid_f)
	{
		printf("...Father just received a signal...\n");
	}
	else 
	{
		if(getpid() == pid_s2)
			printf("...Second son just received a signal...\n");
		else 
			printf("who am i omg\n");
	}*/
}

int main(int argc, char** argv) 
{
	
	signal(SIGUSR1, handler);

	pid_t pid_f, pid_s, pid_g1, pid_g2;

	int fd[2];
	pipe(fd);	

	pid_f = getpid();

	if((pid_s = fork()) < 0) 
	{
		perror("fork");
	}	
	if(pid_s == 0) 
	{ //First child
		pid_s = getpid();
		
		if((pid_g1 = fork()) < 0) 
		{
			perror("fork");
		}
		if(pid_g1 == 0) 
		{ //First grandchild
			pid_g1 = getpid();
			
			close(fd[1]);
			read(fd[0], &pid_g2, sizeof(pid_t));
			
			printf("I'm first grandchild of first father\n");
			printf("\tMy pid is %d\n", pid_g1);
			printf("\tMy brothers pid is %d\n", pid_g2);
			printf("\tMy fathers pid is %d\n", pid_s);
			printf("\tMy grandfathers pid is %d\n", pid_f);
			
			if(kill(pid_f, SIGUSR1) != 0) 
			{
				perror("kill");
			}

			printf("*First grandchild is ending...\n");
			return 0;
		} 
		//First child again
		if((pid_g2 = fork()) < 0) 
		{
			perror("fork");
		}
		if(pid_g2 == 0) 
		{ //Second grandchild
			pid_g2 = getpid();
				
			printf("I'm second grandchild of first father\n");
			printf("\tMy pid is %d\n", pid_g2);
			printf("\tMy brothers pid is %d\n", pid_g1);
			printf("\tMy fathers pid is %d\n", pid_s);
			printf("\tMy grandfathers pid is %d\n", pid_f);

			close(fd[0]);
			write(fd[1], &pid_g2, sizeof(pid_t));
			write(fd[1], &pid_g1, sizeof(pid_t));
			write(fd[1], &pid_g2, sizeof(pid_t));

			printf("*Second grandchild is ending...\n");
			return 0;
		}
		printf("I'm first son\n");
		printf("\tMy pid is %d\n", pid_s);
		printf("\tMy father pid is %d\n", pid_f);
		printf("\tMy sons pids are %d and %d\n", pid_g1, pid_g2);

		int status;
		waitpid(pid_g1, &status, 0);	
		waitpid(pid_g2, &status, 0);	
		return 0;
	}
	//Father again
	printf("...Father is waiting for signal...\n");
	pause();
	printf("...Father recieved signal...\n");
	//Father's second branch**************************************************

	if((pid_s2 = fork()) < 0) 
	{
		perror("fork");
	}	
	if(pid_s2 == 0) 
	{ //Second child
		pid_s2 = getpid();

		printf("...second son is waiting for signal...\n");
		pause(); //letting father to read from pipe previous branch info before my child will write there	
		printf("...second son recieved signal...\n");

		if((pid_g21 = fork()) < 0) 
		{
			perror("fork");
		}
		if(pid_g21 == 0) 
		{ //First grandchild
			pid_g21 = getpid();
			
			close(fd[1]);
			read(fd[0], &pid_g22, sizeof(pid_t));
			
			printf("I'm first grandchild of second father\n");
			printf("\tMy pid is %d\n", pid_g21);
			printf("\tMy brothers pid is %d\n", pid_g22);
			printf("\tMy fathers pid is %d\n", pid_s2);
			printf("\tMy grandfathers pid is %d\n", pid_f);
			
			if(kill(pid_f, SIGUSR1) != 0) 
			{
				perror("kill");
			}

			printf("*First grandchild is ending...\n");
			return 0;
		} 
		//First child again
		if((pid_g22 = fork()) < 0) 
		{
			perror("fork");
		}
		if(pid_g22 == 0) 
		{ //Second grandchild
			pid_g22 = getpid();
				
			printf("I'm second grandchild of second father\n");
			printf("\tMy pid is %d\n", pid_g22);
			printf("\tMy brothers pid is %d\n", pid_g21);
			printf("\tMy fathers pid is %d\n", pid_s2);
			printf("\tMy grandfathers pid is %d\n", pid_f);

			close(fd[0]);
			write(fd[1], &pid_g22, sizeof(pid_t));
			write(fd[1], &pid_g21, sizeof(pid_t));
			write(fd[1], &pid_g22, sizeof(pid_t));

			printf("*Second grandchild is ending...\n");
			return 0;
		}
		printf("I'm second son\n");
		printf("\tMy pid is %d\n", pid_s2);
		printf("\tMy father pid is %d\n", pid_f);
		printf("\tMy sons pids are %d and %d\n", pid_g21, pid_g22);

		int status;
		waitpid(pid_g21, &status, 0);	
		waitpid(pid_g22, &status, 0);	
		return 0;
	}
	//Father again

	close(fd[1]); //only now, cause all children must recieve both fd opened

	read(fd[0], &pid_g1, sizeof(pid_t));
	read(fd[0], &pid_g2, sizeof(pid_t));

	sleep(1); //Assures that second son is already paused; NOT VERY GOOD IDEA
	kill(pid_s2, SIGUSR1); //unpause second son

	printf("...Father is waiting for signal...\n");
	pause();

	read(fd[0], &pid_g21, sizeof(pid_t));
	read(fd[0], &pid_g22, sizeof(pid_t));
	
	printf("I'm grandfather\n");
	printf("\tMy pid is %d\n", pid_f);
	printf("\tMy sons pids are %d and %d\n", pid_s, pid_s2);
	printf("\tMy grandchildren pids are %d and %d and %d and %d\n", pid_g1, pid_g2, pid_g21, pid_g22);

	close(fd[0]);

	int status;
	waitpid(pid_s, &status, 0);
	waitpid(pid_s2, &status, 0);
	return 0;
}
