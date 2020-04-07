#include<unistd.h>
#include<stdio.h>
int main (){
	pid_t pid;
	int count=0;
	pid=fork();
	if(pid<0)	//pid<0 err
		printf("error in fork!");
	else if(pid==0){	//pid==0 child
		printf("i am the child process, my process id is %d\n",getpid()); 
		count++;
	}
	else{	//pid>0 parent
		printf("i am the parent process, my process id is %d\n",getpid()); 
		count++;
	}
	printf("count=%d\n",count);//two return
	return 0;
}
