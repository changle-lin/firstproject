#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

/*
socket
bind
listen
accept
send/recv
*/
#define SERVER_PORT 8888
#define BACKLOG     10

int main(int argc, char **argv)
{
	int iSocketServer;
	int iSocketClient;
	struct sockaddr_in tSocketServeraddr;
	struct sockaddr_in tSocketClientAddr;
	int iRet;
	int iAddrLen;
	unsigned char ucRecvBuf[1000];
	int iClientNum = -1;	
	
	signal(SIGCHLD,SIG_IGN);//防止僵死进程
	
	iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == iSocketServer)
	{
		printf("socket error\n");
		return -1;
	}
	
	tSocketServeraddr.sin _family = AF_INET;
	tSocketServeraddr.sin_port = htons(SERVER_PORT);//host to  net ,short 
	tSocketServeraddr.sin_addr.s_addr = INADDR_ANY;//本机上的所有IP
	memset(tSocketServeraddr.sin_zero, 0 , 8);
	
	iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServeraddr, sizeof(struct sockaddr));
	if(-1 == iRet)
	{
		printf("bind error\n");
		return -1;
	}
	iRet = listen(iSocketServer, BACKLOG);//可以同时检测多少路连接
	if(-1 == iRet)
	{
		printf("listen error\n");
		return -1;
	}
	while(1)
	{
		iAddrLen = sizeof(struct sockaddr);
		iSocketClient = accept(iSocketServer, (struct sockaddr *addr)&tSocketServeraddr,&iAddrLen);
		if(-1 != iSocketClient)
		{
			iClientNum++;
			printf("Get connect from client %d : %s\n", iClientNum, inet_ntoa(tSocketClientAddr.sin_addr));
			if (!fork())//支持多个连接，调用fork函数
			{
				/* 子进程的源码 */
				while (1)
				{
					/* 接收客户端发来的数据并显示出来 */
					iRecvLen = recv(iSocketClient, ucRecvBuf, 999, 0);
					if (iRecvLen <= 0)//没接收到数据
					{
						close(iSocketClient);
						return -1;
					}
					else
					{
						ucRecvBuf[iRecvLen] = '\0';//加上结束符
						printf("Get Msg From Client %d: %s\n", iClientNum, ucRecvBuf);
					}
				}				
			}			
			
		}
	}
	close(iSocketServer);
	return 0;
}