#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t quitflag;

// One signal handler for SIGINT and SIGQUIT

static void sig_int(int signo){
    if(signo == SIGINT)
        printf("\nInterrupted SIGINT!\n");
    else if (signo == SIGQUIT){
        printf("\nInterrupted SIGQUIT\n");
        quitflag = 1;}
}

int main(){
    sigset_t newmask, oldmask, zeromask;

    // Set signal handler
    if(signal(SIGINT,sig_int) == SIG_ERR)
        perror("signal(SIGINT) error.\n");
    if(signal(SIGQUIT,sig_int) == SIG_ERR)
        perror("signal(SIGQUIT) error.\n");

    // Just create two mask sets and do nothing to the process
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGQUIT);

    // Do something to the process with a mask set
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) <0)
        perror("SIG_BLOCK error\n");

    // Try to make a loop
    while(quitflag == 0){
        sigsuspend(&zeromask);
    }

    quitflag = 0;

    // Reset signal mask which unblocks SIGQUIT
    if (sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
        perror("SIG_SETMASK error\n");

    return 0;
}