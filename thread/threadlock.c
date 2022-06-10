#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

pthread_t tid[2];
int counter;
pthread_mutex_t lock;

void *doSomething(void *arg){

    pthread_mutex_lock(&lock);
    counter += 1;

    printf("Job %d started\n",counter);
    // Insert sleep or some long dummy codes to delay Thread
    // It tis take long time, times up and th other thread will run

    for(unsigned long i=0;i<(0xFFFFFFFF);i++);

    printf("Jov %d finished\n",counter);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void){
    int i=0; int err;
    while(i<2){
        err = pthread_create(&tid[i],NULL,&doSomething,(void *)&i);
        if(err < 0){
            fprintf(stderr,"Thread create error\n");
            exit(-1);
        }
        i++;
    }
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);

    return 0;
}