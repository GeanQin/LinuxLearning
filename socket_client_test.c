#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(){
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("192.168.56.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   
    //读取服务器传回的数据
    //char buffer[40];
    //read(sock, buffer, sizeof(buffer)-1);
   
    //printf("Message form server: %s\n", buffer);
   
    char recBuf[1024]={0};
    char sendBuf[1024]={0};
    while(fgets(sendBuf,sizeof(sendBuf),stdin)!=NULL){
        write(sock,sendBuf,sizeof(sendBuf));
        read(sock,recBuf,sizeof(recBuf));
        fputs(recBuf,stdout);
        memset(recBuf, 0, sizeof(recBuf));
    }
    //关闭套接字
    close(sock);

    return 0;
}
