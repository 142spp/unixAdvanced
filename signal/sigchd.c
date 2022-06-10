#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

// Set signal handler
void signalHandler(int signo){
    printf("Catch the signal : %d\n",signo);

    if(signo==SIGCHLD){
        printf("Child process is terminated!\n");
        wait(NULL);
    }
}

int main(){
    signal(SIGALRM,signalHandler);
    signal(SIGUSR1,signalHandler);
    signal(SIGCHLD,signalHandler);

    //!fork() means the process is child process
    if(!fork()){
        //child
        printf("Child process is running...\n");
        sleep(2);
        printf("Child process Sending SIGALRM\n");
        kill(getppid(),SIGALRM);
        sleep(5);
        printf("Child is exiting...\n");
        return 0;
    }
    printf("Parent process is running... PID = %d\n",getpid());
    printf("press ENTER to exit\n");
    getchar();
    printf("Parent process is terminated!\n");
    return 0;
}