#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semp;
int val;

void *wait_func(void *arg){
    printf("Wait_func Thread Started...\n");

    while(1){
        sem_wait(&semp);
        sem_getvalue(&semp,&val);

        printf("Wait_func Thread: sem_value: %d\n",val);
    }
}

int main(void){
    pthread_t pthid = 0;

    if(sem_init(&semp,0,1)){
        fprintf(stderr,"SemInit error\n");
        exit(-1);
    }
    sem_post(&semp);
    sem_post(&semp);
    sem_post(&semp);
    sem_post(&semp);

    sem_getvalue(&semp,&val);

    printf("Main: sem_value: %d\n",val);

    if(pthread_create(&pthid,NULL,wait_func,NULL)!=0){
        fprintf(stderr,"Thread Create error\n");
        exit(-1);
    }
    pthread_join(pthid,NULL);

    exit(EXIT_SUCCESS);
}