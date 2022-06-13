#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void pr_exit(int status){
    if(WIFEXITED(status))
	    printf("normally exited , status: %d",WIFEXITED(status));
}


int main(void){
    pid_t pid;

    
}
