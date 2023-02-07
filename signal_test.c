#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

void sigcb(int signo)
{
    switch (signo)
    {
    case SIGUSR1:
        printf("[%d]Get a signal -- SIGUSR1\n", getpid());
        break;
    case SIGUSR2:
        printf("[%d]Get a signal -- SIGUSR2\n", getpid());
        break;
    }
    return;
}

int main()
{
    pid_t fpid;

    fpid = fork();
    if (fpid < 0)
    {
        printf("error in fork!");
    }
    else if (fpid == 0)
    {
        signal(SIGUSR1, sigcb);
        signal(SIGUSR2, sigcb);
        while (1)
        {
            printf("i am the child process, my process id is %d, father id is %d\n", getpid(), getppid());
            sleep(1);
        }
    }
    else
    {
        while (1)
        {
            printf("i am the parent process, my process id is %d, son id is %d\n", getpid(), fpid);
            sleep(2);
            kill(fpid, SIGUSR1);
            sleep(3);
            kill(fpid, SIGUSR2);
        }
    }
}