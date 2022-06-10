#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

int shmid;  /* shared memory id */
int *honey;

void *sig_int(int signo){ /*signal handler */
    shmdt(honey);  /* detach shared memory */
    sem_unlink("pSem"); /* unlink */
    sem_unlink("cSem");
    if (shmctl(shmid, IPC_RMID, 0) != 0) {  /* shared memory remove */
        perror("Shared memory Delete error ");
        exit(-1);
    }
    printf("Successfully delete Shared memory\n");  
    exit(0); /* Quit process */
}

int main(){
    signal(SIGINT,(sig_t)sig_int); /* Set SIGINT to signal handler */
    key_t key;

    sem_t *semP; /* Prarent semaphore */
    sem_t *semC; /* Child semaphore */

    key = ftok("honeybee",'R'); /* Create key */
    shmid = shmget(key,sizeof(int),0644|IPC_CREAT); /* Create Shared memory */
    honey = shmat(shmid,(void *)0,0);  /* attach shared memory */

    semP = sem_open("pSem",O_CREAT|O_EXCL,0644,0);  /* open semaphore , value is 0 */
    semC = sem_open("cSem",O_CREAT|O_EXCL,0644,5);  /* open semaphore , value is 5 */

    pid_t pid; /* fork * 5 */
    for(int i=0; i<5; i++) {
        pid = fork();   
        if(pid==0) break;
    }

    if(pid==0){ /* Child */
        pid_t pid = getpid();
        while(1){ /* repeatly */
            sem_wait(semC); /* wait for semC > 0 and decrease semC
                            but don't need because all five bees work concurrently*/
            *honey = *honey + 1;  /* (1)gather 1 honey */
            srand(*honey + clock());  /* create random value */
            unsigned int randtime = rand()%1000001; 
            if(randtime < 100000) randtime = 100000;  /* fix minvalue */
            printf("[%d] Bee filled the pot! current: %d  %03d(ms)\n",pid,*honey,randtime/1000); /* (2) put in the pot */
            sem_post(semP); /* Awake the bear (Parent) iff increase Parent semaphore */
            usleep(randtime);  /* (3) sleep for random value */
        }
    }else{  /* Parent */
        while(1){ 
            /* semP's initial value is 0, so if the bees(Child process) don't wake(increase semP), it won't work */
            sem_wait(semP); /* wait for child process increase semP and decrease semP */
            if(*honey >= 20){ /* if pot is full */
                *honey = 0;  /* eat all honey */
                printf("\nThe Bear Eats all! current: %d\n\n",*honey); /* print message */
            }
            sem_post(semC); /* increase Child semaphore */
        }
    }
    return 0;
}