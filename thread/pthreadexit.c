#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *thread_function(void *data){
    static int retval = 999;
    int i=0;
    int mycount = *((int *)data);
    while(1){
        if(i==mycount){
            pthread_exit((void *)&retval);
        }
        printf("Thread Running...%d\n",i);
        i++;
        sleep(1);
    }
}

int main(){
    pthread_t p_thread;
    void *threadret = NULL;
    int ret,count=3;
    ret = pthread_create(&p_thread,NULL,thread_function,(void *)&count);
    
    if(ret < 0){
        fprintf(stderr,"Thread create error: ");
        exit(-1);
    }
    
    pthread_join(p_thread,&threadret);
    printf("Thread exit code: %d\n",*((int *)threadret));
    printf("Main thread is finished...\n");
}