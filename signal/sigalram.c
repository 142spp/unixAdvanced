#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int count= 0;

void mySignalHandler(int signo){
    printf("Alarm %d [signal number: %d]", count++,signo);
    alarm(2);
}

int main(){
    //Register Sigalrm handler
    signal(SIGALRM,mySignalHandler);
    alarm(2);
    // I want to trigger Sigalrm forever
    while(1);

    return 0;
}
