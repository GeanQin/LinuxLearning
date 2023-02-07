#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void sigcb(int signo)
{
    switch (signo)
    {
    case SIGHUP:
        printf("Get a signal -- SIGHUP\n");
        break;
    case SIGINT:
        printf("Get a signal -- SIGINT\n");
        break;
    case SIGQUIT:
        printf("Get a signal -- SIGQUIT\n");
        break;
    }
    return;
}

int main()
{
    signal(SIGHUP, sigcb);
    signal(SIGINT, sigcb);
    signal(SIGQUIT, sigcb);
    for (;;)
    {
        printf("my id=%d\n", getpid());
        sleep(5);
    }
}