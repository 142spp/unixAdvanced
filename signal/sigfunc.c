#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define MY_SIGNAL 2
#define MAX_SIGNAL 32

#define SIG_DDD_X (void (*) (int)) -1
#define SIG_DDD_Y (void (*) (int)) 0
#define SIG_DDD_Z (void (*) (int)) 1

typedef void Sigfunc(int);

struct {
    int signo;
    Sigfunc* signal_handler;
} sigtable[MAX_SIGNAL];

Sigfunc* signal_tak(int signo, Sigfunc my_signal ){
    sigtable[signo].signo = signo;
    sigtable[signo].signal_handler=my_signal;

    return SIG_DDD_Z;
}

void sig_usr(int signo){
    printf("A signal [%d] occurs\n",signo);
}

int main(){
    int signo;
    if (signal_tak(MY_SIGNAL,sig_usr)==SIG_DDD_Z)
        printf("Return 1\n");

    signo = MY_SIGNAL;
    sigtable[MY_SIGNAL].signal_handler(signo);
}