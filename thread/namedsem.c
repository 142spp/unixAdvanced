#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>

#define LOOP 10

void shmRemove(int shmid, int *data){
    shmdt(data);
    if (shmctl(shmid, IPC_RMID, 0) != 0) {
        perror("Shared memory Delete error ");
        exit(-1);
    }
    printf("Successfully delete Shared memory\n");
}

int main() {

    // Shared memory
    key_t key;
    int shmid;
    int *data;
    int prev = 1000;

    // Semaphore Heap **but can Stack, Global**
    sem_t *semP = (sem_t *)malloc(sizeof(sem_t));
    sem_t *semC = (sem_t *)malloc(sizeof(sem_t));

    int pShared = 1;
    unsigned int valueP = 1;
    unsigned int valueC = 0;

    key = ftok("namedsem.c", 'R');
    shmid = shmget(key, sizeof(int), 0644 | IPC_CREAT);

    data = shmat(shmid, (void *)0, 0);
    if (data == (int *)(-1)) {
        fprintf(stderr, "Shared memory Attach error\n");
        exit(-1);
    }

    // Named Semaphore
    semP = sem_open("pSem", O_CREAT | O_EXCL, 0644, valueP);

    if (semP == SEM_FAILED){
        shmRemove(shmid,data);
        perror("Sem Open [pSem] ");
        exit(-1);
    }

    semC = sem_open("cSem",O_CREAT|O_EXCL,0644,valueC);

    if(semC == SEM_FAILED){
        shmRemove(shmid,data);
        perror("Sem Open [cSem] ");
        exit(-1);
    }
    
    // Fork
    pid_t child_pid;
    
    if((child_pid = fork()) < 0){
        perror("Fork failed\n");
        exit(-1);
    } if(child_pid ==0){    // Child process
        for(int i=0; i<LOOP; i++){
            sem_wait(semP);
            *data = *data + 1;
            printf("Child data: %d\n",*data);
            sleep(2);
            sem_post(semC);
        }
    } else{                 // Parent process
        for(int i=0; i<LOOP; i++){
            sem_wait(semC);
            *data = *data - 1;
            printf("Parent data: %d\n",*data);
            sleep(1);
            sem_post(semP);
        }
    }
    shmRemove(shmid,data);
    sem_unlink("pSem");
    sem_unlink("cSem");

    return 0;
}