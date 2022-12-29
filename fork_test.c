#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t fpid;
	int count = 0;
	int status = 0;

	/* fork()
	 * ret: 若成功调用一次则返回两个值，子进程返回0，父进程返回子进程ID；否则，出错返回-1
	 */

	while (1)
	{
		fpid = fork();
		if (fpid < 0)
		{
			printf("error in fork!");
		}
		else if (fpid == 0)
		{
			printf("i am the child process, my process id is %d\n", getpid());
			count++;
			sleep(5);
			_exit(0);
		}
		else
		{
			printf("i am the parent process, my process id is %d\n", getpid());
			count++;
			sleep(2);
			waitpid(-1, &status, WNOHANG);
		}
		printf("count: %d\n", count);
	}

	return 0;
}