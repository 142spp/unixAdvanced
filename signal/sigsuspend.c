#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>


void pr_mask(const char *str){
    sigset_t sigset;

    // Without this code, sigset is located on the stack.
    // The sigset has some garbage values.
    // That's why we need to initialize 0s for all signals.
    // So we do sigprocmask(0,NULL...

    // 앞의 두 인자는 필요가 없고, 단지 현재 stack 에 있는 signal set 을 sigset 에 저장하기 위한 용도
    if(sigprocmask(SIG_BLOCK,NULL,&sigset) < 0)
        perror("sigprocmask error : ");

    printf("%s",str);

    if(sigismember(&sigset,SIGINT))  printf("SIGINT \n");
    if(sigismember(&sigset,SIGQUIT)) printf("SIGQUIT\n");
    if(sigismember(&sigset,SIGALRM)) printf("SIGALRM\n");
    if(sigismember(&sigset,SIGUSR1)) printf("SIGUSR1\n");
    
    printf("\n");
}

void sig_int(int signo){
    pr_mask("\nIn sig_int: ");
}

int main(void){
    sigset_t newmask,oldmask,waitmask;

    pr_mask("Program Start: ");

    if(signal(SIGINT, sig_int)==SIG_ERR)
        perror("SIGINT ERROR");

    pr_mask("After calling sig_int: ");

    sigemptyset(&waitmask);
    sigaddset(&waitmask,SIGUSR1);

    sigemptyset(&newmask);
    sigaddset(&newmask,SIGINT);

    // Block SIGINT(Ctrl+C) and save current signal mask
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
        perror("sigprocmaks error");

    pr_mask("int crticial region: ");

    // Pause, allowing all signals except SIGUSR1
    if(sigsuspend(&waitmask) != -1)
        perror("sigsuspend error");
    
    pr_mask("After return from suspend: ");

    // Reset signal mask which unblocks SIGINT
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        perror("SIG_SETMASK error");

    pr_mask("Program Exit...: ");
    return 0;
}