/*#include <iostream.h>
using namespace std;//iostream.h该头文件没有定义全局命名空间，必须使用namespace std
*/
//server.c
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
 
void error_handling(char* message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
	
 
int main()
{
	int listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd<0)
	error_handling("socket");
	
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(5188);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	int on=1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0)
	error_handling("socket");
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	error_handling("bind");
 
	if(listen(listenfd,SOMAXCONN)<0)
	error_handling("listen");
 
	struct sockaddr_in peeraddr;
	socklen_t peerlen=sizeof(peeraddr);
 
	int conn;
	//if((conn=accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
	//error_handling("accept");	
	
 
	//printf("IP=%s port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));//打印对等方的IP地址和端口号
	
	//char recvbuf[1024];
	//while(1)
	//{
	//	memset(recvbuf,0,sizeof(recvbuf));
	//	int ret=read(conn,recvbuf,sizeof(recvbuf));
	//	fputs(recvbuf,stdout);
	//	write(conn,recvbuf,ret);
	//}
	
//close(conn);close(listenfd);
//	return 0;
	//}
 
  pid_t pid;
  while(1)
{
  if((conn=accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)
	error_handling("accept");
  
  printf("ip=%s  port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
 
 pid=fork();
 
 if(pid==-1)
 error_handling("fork");
 
 if(pid==0)
{
  close(listenfd);//子进程关闭监听套接字
  
 char recvbuf[1024];
 
 while(1)
{
  memset(recvbuf,0,sizeof(recvbuf));
   int ret=read(conn,recvbuf,sizeof(recvbuf));
    fputs(recvbuf,stdout);
    write(conn,recvbuf,ret);
}
}
 
else close(conn);//父进程关闭连接套接字
}
 
return 0;
}
 
	