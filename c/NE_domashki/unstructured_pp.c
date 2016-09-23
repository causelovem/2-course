#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <sys/time.h>

#include <signal.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

#define COMMAND_SEND_BALL   0
#define COMMAND_FINISH_WORK 1

#define SEM_PING_PONG 0
#define SEM_CONNECTED 1

typedef struct
{
    unsigned int ball;
    int command;
} Shared_memory;

void ctrl_c_handler(int sig);

struct sembuf op_sem_up[1]         = {{SEM_PING_PONG,  1, 0}};
struct sembuf op_sem_down[1]       = {{SEM_PING_PONG, -1, 0}};
struct sembuf op_sem_connect[1]    = {{SEM_CONNECTED,  1, 0}};
struct sembuf op_sem_disconnect[1] = {{SEM_CONNECTED, -1, 0}};
struct sembuf op_sem_wait_all[1]   = {{SEM_CONNECTED,  0, 0}};

int shm_id, sem_id;

Shared_memory *mem;


void ctrl_c_handler(int sig)
{
    mem->command = COMMAND_FINISH_WORK;
    semop(sem_id,op_sem_up,1);
}


int main()
{
    key_t key;
    
    int shm_owner=0;
    int sem_owner=0;

    unsigned int      local_ball=0;
    unsigned long int counter=0;
   
   
    key=ftok("/bin/sh",45);



    shm_id=shmget(key,sizeof(Shared_memory),IPC_CREAT|IPC_EXCL|0666);
    if(shm_id == -1)
    {
        shm_id=shmget(key,sizeof(Shared_memory),0);
        if(shm_id == -1)
        {
            perror("Shared memory in ping-pong");
            return 1;
        }
    }
    else
    {
        shm_owner=1;
    }

    sem_id=semget(key,2,IPC_CREAT|IPC_EXCL|0666);
    if(sem_id == -1)
    {
        sem_id=semget(key,2,0);
        if(sem_id == -1)
        {
            perror("Semaphore in ping-pong");
            return 1;
        }
        semop(sem_id,op_sem_connect,0);
    }
    else
    {
        sem_owner=1;
    }

    signal(SIGINT,ctrl_c_handler);

    mem=(Shared_memory *)shmat(shm_id,NULL,0);
    if(mem == (void *)-1)
    {
        perror("attach to shared memory in ping-pong");
        return 2;
    }

    if(shm_owner)
    {
        mem->ball=0;
        mem->command=COMMAND_SEND_BALL;
        semop(sem_id,op_sem_up,1);
    }

    /*
     * Инициализация датчика случайных чисел.
     */
    {
        struct timeval tv;

        gettimeofday(&tv,NULL);
        srand((unsigned int)(getpid()+tv.tv_usec));
    }



    while(1)
    {
        semop(sem_id,op_sem_down,1);
        /*
         * Типо критическая секция
         * начало
         */
            
            if(mem->command == COMMAND_FINISH_WORK)
            {
               if(!shm_owner)
               {    
                    /*
                     * Уменьшаем семафор, тем самым
                     * информируем, что процесс всё прочитал
                     * из разделяемой памяти. 
                     */
                    semop(sem_id,op_sem_disconnect,1);
               }
               /*
                * покидаем критическую секцию
                */
               semop(sem_id,op_sem_up,1);
               break; 
            }

            local_ball=mem->ball;
            counter++;
            mem->ball++;

            /*
             * Усыпляем процесс на от 0 до 10 секунд.
             */
            sleep(rand()%10);
        /*
         * конец критической секции
         */
        semop(sem_id,op_sem_up,1);
    }

    semop(sem_id,op_sem_wait_all,1);

    if(sem_owner)
    {
        semctl(sem_id,IPC_RMID,0);
    }

    shmdt(mem);
    if(shm_owner)
    {       
        shmctl(shm_id,IPC_RMID,NULL);
    }

    printf("Ball value: %u\nMem accessed: %lu ones\n",local_ball,counter);

    return 0;
}

