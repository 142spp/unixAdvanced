#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "chatshm.h"

int main(int argc,char** argv){
    int shmid;
    char userID[20];
    CHAT_INFO *chatinfo = NULL;

    void *shmaddr = NULL;

    if(argc<2){
        fprintf(stderr,"Usage: ./writeshm (UserID)\n");
        exit(-1);
    }
    strcpy(userID,argv[1]);
    shmid = shmget((key_t)3836,sizeof(CHAT_INFO),0666|IPC_CREAT|IPC_EXCL);
    if(shmid<0){
        shmid = shmget((key_t)3836,sizeof(CHAT_INFO),0666);
    }

    if(shmid<0) {
        fprintf(stderr,"an error occur\n");
        exit(-1);
    }

    shmaddr = shmat(shmid,NULL,0666);
    if(shmaddr < NULL){
        fprintf(stderr,"shmat attach is faild: ");
        exit(-1);
    }
    chatinfo = (CHAT_INFO *) shmaddr;

    printf("------------Chat Message-----------\n");
    while(1){
        char inputStr[40];
        printf("Enter your message: ");
        fgets(inputStr,40,stdin);
        strcpy(chatinfo->userID,userID);
        chatinfo->messageTime++;
        strcpy(chatinfo->message,inputStr);
    }
    return 0;
}