#include<unistd.h>
#include<stdio.h>
int main (){
	pid_t pid;
	pid=fork();
	if(pid<0){
		printf("error in fork!");
		exit(0);
	}else if(pid>0){
		exit(0);
	}
	pid = setsid();
	if(pid<0){
		printf("error in setsid!");
		exit(0);
	}
	printf("Hello!");
	return 0;
}
