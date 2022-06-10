#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    int shmid = shmget((key_t)1231,sizeof(int),0644|IPC_CREAT);
    int *data = shmat(shmid,NULL,0644); // xrw xrw xrw = 011 010 010;

    // int *data = (int *)malloc(sizeof(int)); 
    // *data = 0;   that is why we use shared memory;
    
    /* fork copy all the data of parent, also pointer do. 
       because process use virtual memory, these pointer are same 
       but indicate different physical address. so value is different
       **CPU use Virtual memory/address**
    */
    
    pid_t pid = fork();
    while(1){
        if(pid == 0){
            *data += 1;
            printf("Child: %d\n",*data);
        } else {
            *data += 2;
            printf("Parent: %d\n",*data);
        }
        sleep(2);
    }
}