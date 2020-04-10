#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("192.168.56.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //进入监听状态，等待最多20用户发起请求
    listen(serv_sock, 20);

    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    int clnt_sock = 0;
    while(1){
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        printf("addr=%s,port=%d\n", inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port));
        int pid=fork();
        if(pid==0){
            close(serv_sock);
            char buf[1024]={0};
            while(1){
                read(clnt_sock,buf,sizeof(buf));
                fputs(buf,stdout);
                write(clnt_sock,buf,sizeof(buf));
            }
        }else if(pid>0){
            close(clnt_sock);
        }else{
            close(clnt_sock);
            close(serv_sock);
            exit(0);
        }
    }
   
    //关闭套接字
    close(clnt_sock);
    close(serv_sock);

    return 0;
}
