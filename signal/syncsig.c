/* 41 ~ 43 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 100

static volatile sig_atomic_t sigflag; // set nonzero by sig handler
static sigset_t newmask, oldmask,zeromask;

// One signal handler for SIGUSR1 and SIGUSR2
static void sig_usr(int signo){
    sigflag = 1;
}

void charactatime(char* str){
    char *ptr;
    int c;
    
    // When we set stdout (standard output -> monitor) unbuffered.
    // parent and child process can cut in the stdout easily 
    
    setbuf(stdout,NULL); // set unbuffered

    // c!=0 means not null zero
    // string ends with null zero
    for(ptr=str; (c=*ptr++)!=0; )        
        putc(c,stdout);
}

void TELL_WAIT(){
    if(signal(SIGUSR1, sig_usr) == SIG_ERR)
        perror("signal(SIGUSR1) error\n");
    if(signal(SIGUSR2, sig_usr) == SIG_ERR)
        perror("signal(SIGUSR2) error\n");
    // Initialize mask set
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGUSR1);
    sigaddset(&newmask,SIGUSR2);

    // Block SIGUSR1 and SIGUSR2, and save current signal mask
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
        perror("SIG_BLOCK error\n");
    
}

void WAIT_PARENT(){
    while(sigflag == 0)
        sigsuspend(&zeromask);
    
    // Back to the previous signal mask (SIG_BLOCK with SIGUSR1 and SIGUSR2)
    // Still blocking SIGUSR1 and SIGUSR2 when we access sigflag...
    // Why? sigflag is share by parent and child process and sigflag can be 
    // changed at any time by signals SIGUSR1 and SIGUSR2..
    
    sigflag = 0;

    // After we successfully change the value of sigflag into 0.
    // Now we need to allow SIGUSR1 and SIGUSR2 for the next synchronization
    // Parent and child processes can receive the SIGUSR1 & SIGUSR2 from each other
    
    // Reset signal mask to original value
    // We need to allow sending signal SIGUSR1 and SIGUSR2...
    // Why? - when we are finished with crtl + c

    // If you do not allow further SIGUSR1 and SIGUSR2.. may be getting into the
    // deadlock situation.. 

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        perror("SIG_BLOCK error\n");
}
void WAIT_CHILD(){
    while(sigflag==0)
        sigsuspend(&zeromask);

    sigflag =0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        perror("SIG_BLOCK error\n");
}

void TELL_PARENT(pid_t pid){
    kill(pid,SIGUSR2);
}

void TELL_CHILD(pid_t pid){
    kill(pid,SIGUSR1);
}

int main(){
    pid_t pid;
    char str[MAX] = {'\0'};

    // Initialize signal mask and handler
    TELL_WAIT();
    pid = fork();

    if(pid<0)
        perror("fork error");
    else if (pid == 0){
        //Child
        for (int i=0; i<10; i++){
            WAIT_PARENT();
            charactatime("Output from child: ");
            sprintf(str,"%d",i);
            charactatime(str);
            charactatime("\n");        
            TELL_PARENT(getppid());
        }
    }
    else {
        //Parent
        for (int i=0; i<10; i++){
            charactatime("Output from parent: ");
            sprintf(str,"%d",i);
            charactatime(str);
            charactatime("\n");            
            TELL_CHILD(pid);
            WAIT_CHILD();
        }
    }
    return 0;
}