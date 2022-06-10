#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

static void stq_quit(int signo)
{
    printf("Catch SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
        perror("Cannot reset SIGQUIT\n");
    return;
}

int main()
{
    sigset_t newmask, oldmask, pendmask;

    if (signal(SIGQUIT, stq_quit) == SIG_ERR)
        perror("Cannot catch SIGQUIT\n");

    sigemptyset(&newmask); // just generate empty sigset. no imply
    sigaddset(&newmask, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        perror("SIG_BLOCK Error: ");
    sleep(5);

    if (sigpending(&pendmask) < 0)
        perror("sigpending error: ");

    if (sigismember(&pendmask, SIGQUIT) == 1)
        printf("SIGQUIT is pending\n");

    if(sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
        perror("SIG_SETMASK Error: ");

    printf("SIGQUIT is unblocked \n");

    sleep(5);

    //* SIGQUIT here will terminate with core file

    //for (;;);

    return 0;
}