#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "chatshm.h"

int main(){
    int shmid;
    CHAT_INFO *chatinfo = NULL;

    void *shmaddr = NULL;

    shmid = shmget((key_t)3836,sizeof(CHAT_INFO),0666|IPC_CREAT|IPC_EXCL);
    
    // If shm have already exist... just het shm
    if(shmid<0){
        shmid = shmget((key_t)3836,sizeof(CHAT_INFO),0666);
    }
    if(shmid<0) {
        fprintf(stderr,"Shm Get Error\n");
        exit(-1); 
    }

    shmaddr = shmat(shmid,NULL,0666);
    if(shmaddr < (void *)0){
        fprintf(stderr,"Shm Attach Error\n");
        exit(-1);
    }
    chatinfo = (CHAT_INFO *) shmaddr;

    printf("------------Chat Message-----------\n");
    while(1){
        printf("[%s]%ld: %s",
        chatinfo->userID,
        chatinfo->messageTime,
        chatinfo->message);
        if(!strcmp(chatinfo->message,"/exit\n")){
            printf("%s is out\n",chatinfo->userID);
            break;
        }
        sleep(3);
    }
    return 0;
}