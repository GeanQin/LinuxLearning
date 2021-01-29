#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>

int main()
{
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    
	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "socket_server_test");

	/*
	 * 链接至服务端
	 * int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	 * 第一个参数即为客户端的socket描述字
	 * 第二参数为服务器的socket地址
	 * 第三个参数为socket地址的长度
	 */
	int result = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
	if(result == -1)
	{
		perror("connect failed: ");
		exit(1);
	}

	char str[256];
	read(sockfd, str, 256);
	printf("get from server: %s\n", str);

	close(sockfd);

	return 0;
}  

