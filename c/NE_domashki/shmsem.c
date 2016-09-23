#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define PERM 0666
#define MAXLEN 256

struct sembuf unlock[1] = { {0,1,0} };
struct sembuf start_value[1]={{3,2,0}};
struct sembuf a1[1]={{3,-1,IPC_NOWAIT}};
struct sembuf a2[1]={{3,1,0}};

struct sembuf client_post_wait[3] = { {0,-1,0}, {1,1,0}, {2, 0, 0} };
struct sembuf server_ans_wait[3] = { {0,-1,0}, {1,-1,0}, {2,1,0} };
struct sembuf client_req_wait[2] = { {0,-1,0},           {2,-1,0} };

int shmd;
int semd;
char* memory;

int cleaner();
int server();
int client(char* str);

int main(int argc,char** argv)
{
	if(argv[1][0]=='s') server();
	else if(argv[1][0]=='c') client(argv[2]);
	return 0;
}

int server()
{
	printf("Server launched\n");
	key_t  key = ftok("shmsem.c", 'a');
	
	semd = semget(key, 4, PERM | IPC_CREAT | IPC_EXCL);
	shmd = shmget(key, MAXLEN*sizeof(char), PERM | IPC_CREAT | IPC_EXCL);

	memory = (char*)shmat(shmd,0,0);
	
	if(memory==(void*)-1)
	{
		printf("Error getting shared memory, running cleaner:\n");
		cleaner();
		return -1;
	}
	
	semop(semd,unlock,1);
	semop(semd,start_value,1);
	
	char input[32];
	for(;;)
	{
		printf("Server waits for request to process... \n");
		semop(semd,server_ans_wait,3);
		
		strcpy(input, memory);
		printf("Server reads: %s\n",input);
		
		if(strcmp(input, "shutdown") == 0) {
			sprintf(memory, "OKAY\n");				
			semop(semd,unlock,1);
			break;
		}
		
		int i;
		for(i = 0; i <10; ++i) {
			printf("processing... %d %%\n", i * 10);
			sleep(1);			
		}		
		sprintf(memory, "I've heard you!\n");		
		
		semop(semd,unlock,1);
	}
	
	//Removing shared objects from memory
	semctl(semd, 0, IPC_RMID, 0);
	shmctl (shmd , IPC_RMID , 0);
	
	return 0;
}

int client(char* str)
{
	printf("Client launched\n");
	key_t  key = ftok("shmsem.c", 'a');
	
	semd = semget(key, 4, PERM | IPC_CREAT);
	
	if (semop(semd,a1,1)==-1)
	{
		printf("vse zanyato\n");
		return 0;
	}
	shmd = shmget(key, MAXLEN*sizeof(char), PERM | IPC_CREAT);
	memory = (char*)shmat(shmd,0,0);
	
	if(memory==(void*)-1) {
		perror("Client memory get error");
		return -1;
	}	

	semop(semd,client_post_wait,3);
	
	printf("Client writes: %s\n", str);
	strcpy(memory, str);
	
	semop(semd,unlock,1);
	
	semop(semd,client_req_wait,2);
	
	printf("Server answered: %s", memory);
	
	semop(semd,unlock,1);
	semop(semd,a2,1);
	
	return 0;
}

int cleaner()
{
	printf("Cleaner launched\n");
	key_t  key = ftok("shmsem.c", 'a');
	
	semd = semget(key, 4, PERM | IPC_CREAT);
	shmd = shmget(key, MAXLEN*sizeof(char), PERM | IPC_CREAT);
	
	//Removing shared objects from memory
	semctl(semd, 0, IPC_RMID, 0);
	shmctl (shmd , IPC_RMID , 0);
	
	printf("Cleaner finished! Relaunch server\n");
	return 0;
}
