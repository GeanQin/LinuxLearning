#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(){
	/* 
	 * 断开之前的socket文件
	 */
	unlink("socket_server_test");

	/* 
	 * 创建套接字
	 * int socket(int domain, int type, int protocol);
	 * domain:
	 * 	AF_INET 这是大多数用来产生socket的协议，使用TCP或UDP来传输，用IPv4的地址
	 * 	AF_INET6 与上面类似，不过是来用IPv6的地址
	 * 	AF_UNIX 本地协议，使用在Unix和Linux系统上，一般都是当客户端和服务器在同一台及其上的时候使用
	 * type:
	 * 	SOCK_STREAM 这个协议是按照顺序的、可靠的、数据完整的基于字节流的连接。这是一个使用最多的socket类型，这个socket是使用TCP来进行传输。
	 * 	SOCK_DGRAM 这个协议是无连接的、固定长度的传输调用。该协议是不可靠的，使用UDP来进行它的连接。
	 * 	SOCK_SEQPACKET 这个协议是双线路的、可靠的连接，发送固定长度的数据包进行传输。必须把这个包完整的接受才能进行读取
	 * 	SOCK_RAW 这个socket类型提供单一的网络访问，这个socket类型使用ICMP公共协议。（ping、traceroute使用该协议）
	 * 	SOCK_RDM 这个类型是很少使用的，在大部分的操作系统上没有实现，它是提供给数据链路层使用，不保证数包的顺序
	 * protocol:
	 * 	0 默认协议
	 * 返回值：
	 * 	成功返回一个新的文件描述符，失败返回-1，设置errno
	 */
	int server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	/* 
	 * Unix域套接口地址结构
	 * struct sockaddr_un
	 * {
	 * 	sa_family_t sun_family;
	 * 	char sun_path[104];
	 * };
	 */
	struct sockaddr_un server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "socket_server_test");

	/*
	 * 与本地文件进行绑定
	 * int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	 * sockfd：
	 * 	即socket描述字，它是通过socket()函数创建了，唯一标识一个socket。bind()函数就是将给这个描述字绑定一个名字。
	 * addr：
	 * 	一个const struct sockaddr *指针，指向要绑定给sockfd的协议地址。
	 * addrlen：
	 * 	对应的是地址的长度。
	 */
	bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	/* 
	 * 进入监听状态
	 * int listen(int sockfd, int backlog);
	 * 第一个参数即为要监听的socket描述字
	 * 第二个参数为相应socket可以排队的最大连接个数
	 */
	listen(server_sockfd, 1);

	struct sockaddr_un client_addr;
	socklen_t client_addr_size = sizeof(client_addr);

	/*
	 * 接受客户端的连接
	 * int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	 * 第一个参数为服务器的socket描述字
	 * 第二个参数为指向struct sockaddr *的指针，用于返回客户端的协议地址
	 * 第三个参数为客户端协议地址的长度
	 */
	int client_sockfd;
	char str[] = "Hello!";
	
	while(1){
		client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &client_addr_size);

	/* 
	 * 数据交换
	 * #include <unistd.h>
	 * ssize_t read(int fd, void *buf, size_t count);		
	 * read：
	 * 	从fd中读取内容,返回字节数
	 * ssize_t write(int fd, const void *buf, size_t count)；
	 * write:
	 * 	将buf中的nbytes字节内容写入文件描述符fd。成功时返回写的字节数。失败时返回-1。
	 *
	 * #include <sys/types.h>
	 * #include <sys/socket.h>
	 * ssize_t send(int sockfd, const void *buf, size_t len, int flags);
	 * ssize_t recv(int sockfd, void *buf, size_t len, int flags);
	 * ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
	 * ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
	 * ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
	 * ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
	 */
		write(client_sockfd, str, sizeof(str));
	}

	close(client_sockfd);
	close(server_sockfd);

	return 0;
}
