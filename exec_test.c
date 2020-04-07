#include<stdio.h>

int main(void){
	execl("/bin/ls","ls","-a","-l", NULL); //绝对路径
	printf("execl ok!\n");	//will not print

	//execlp("ls", "ls", "-a", "-l", NULL);	//从PATH中取

	//int  execve(const  char  *filename, char *const argv[], char *const envp[]);
	//exec函数是库函数，这个是系统函数；exec函数最终都是调用这个函数实现的
}
