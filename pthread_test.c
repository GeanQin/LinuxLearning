#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;
int num = 0;

void *test1(void *arg){
	while(1){
		pthread_mutex_lock(&lock);
		if(num > 0){
			pthread_cond_signal(&cond);
		}else{
			num = num + 2;
			printf("thread1： %d\n", num);
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
	return 0;
}

void *test2(void *arg){
	while(1){
		pthread_mutex_lock(&lock);
		if(num == 0){
			printf("thread2 wait\n");
			pthread_cond_wait(&cond, &lock);
		}
		num = num - 1;
		printf("thread2： %d\n", num);
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
	return 0;
}

static int tmp_idx = 0; 
void *test(void *arg){
	int idx = *(int *)arg;
	while(1)
	{
		if (tmp_idx != idx)
		{
			pthread_mutex_lock(&lock);
			printf("thread%d wait\n", idx);
			pthread_cond_wait(&cond, &lock);
			pthread_mutex_unlock(&lock);
		}
		sleep(1);
		printf("%d complete\n", idx);
		tmp_idx = 1 - idx;

		pthread_mutex_lock(&lock);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&lock);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int ret = -1;
	pthread_t th1, th2;
	pthread_t th[2];
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);

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
	// ret = pthread_create(&th2, NULL, test2, NULL);
	// if( ret != 0 ){
	// 	printf("Create thread2 error!\n");
	// 	return -1;
	// }
	// sleep(1);
	// ret = pthread_create(&th1, NULL, test1, NULL);
	// if( ret != 0 ){
	// 	printf("Create thread1 error!\n");
	// 	return -1;
	// }

	int idx0 = 0;
	ret = pthread_create(&th[idx0], NULL, test, &idx0);
	if( ret != 0 ){
		printf("Create thread%d error!\n", idx0);
		return -1;
	}
	int idx1 = 1;
	ret = pthread_create(&th[idx1], NULL, test, &idx1);
	if( ret != 0 ){
		printf("Create thread%d error!\n", idx1);
		return -1;
	}
	printf("This is the main process.\n");
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
	// pthread_join(th1, &thread_ret);
	// printf("thread1_ret = %d.\n", thread_ret);
	// pthread_join(th2, &thread_ret);
	// printf("thread2_ret = %d.\n", thread_ret);
	pthread_join(th[0], &thread_ret);
	printf("thread_ret = %d.\n", thread_ret);
	pthread_join(th[1], &thread_ret);
	printf("thread_ret = %d.\n", thread_ret);
	return 0;
}
