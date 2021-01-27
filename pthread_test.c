#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct args{
	unsigned char *buff;
	int count;
};

void *test(void *arg){
	struct args *temp = (struct args*)arg;
	char str[5];
	for(int i=0;i<temp->count;i++){
		strncpy(str,temp->buff+i*4,4);
		str[4]='\0';
		printf("%d is [%s]\n", i+1, str);
	}
	/*
	 * 测试死循环
	 * while(1){
	 * 	sleep(1000);
	 * }
	 */
	return 0;
}

int main(int argc, char *argv[])
{
	int ret = -1;
	pthread_t th;
	struct args *arg = (struct args*)malloc(sizeof(struct args));
	arg->buff = "qwer1234";
	arg->count = 2;
	void *thread_ret = NULL;
	/*
	 * pthread_create是UNIX环境创建线程函数 
	 * int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg); 
	 * 第一个参数为指向线程标识符的指针。
	 * 第二个参数用来设置线程属性。
	 * 第三个参数是线程运行函数的起始地址。
	 * 最后一个参数是运行函数的参数。
	 * 若成功则返回0，否则返回出错编号
	 */
	ret = pthread_create( &th, NULL, test, (void*)arg );
	if( ret != 0 ){
		printf( "Create thread error!\n");
		return -1;
	}
	printf( "This is the main process.\n" );
	/*
	 * 如果线程5s不结束，我就直接退程序
	 * sleep(5);
	 * exit(0);
	 */
	/* 
	 * 函数pthread_join用来等待一个线程的结束。
	 * void pthread_join(pthread_t thread,void ** retval);
	 * 第一个参数为线程标识符。
	 * 第二个参数为一个用户定义的指针。它可以用来存储被等待线程的返回值。
	 * 如果执行成功，将返回0，如果失败则返回一个错误号。
	 */
	pthread_join(th, &thread_ret);
	printf( "thread_ret = %d.\n", thread_ret );
	return 0;
}
