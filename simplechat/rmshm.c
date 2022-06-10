#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "chatshm.h"

int main(){
    int shmid;

    shmid = shmget((key_t)3836,sizeof(CHAT_INFO),0666);

    if(shmid<0) {
        fprintf(stderr,"an error occur\n");
        exit(-1);
    }
    if(shmctl(shmid,IPC_RMID,0)<0){
        fprintf(stderr,"Failed to delete shared memory\n");
        exit(-1);
    }
    else{
        printf("Successfully delete shared memory\n");
    }    

    return 0;
}