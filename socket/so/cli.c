//client.c
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
	int sock;
	sock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sock<0)
	error_handling("sock");
 
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(5188);
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	if(connect(sock,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	error_handling("connect");
	
	char sendbuf[1024]={0};
	char recvbuf[1024]={0};
	
	while(fgets(sendbuf,sizeof(sendbuf),stdin)!=NULL)
	{
		write(sock,sendbuf,strlen(sendbuf));   //不是sizeof的原因:sizeof是1024，strlen是读多少字节就取多少个
		read(sock,recvbuf,sizeof(recvbuf));
		fputs(recvbuf,stdout);
		
		memset(sendbuf,0,sizeof(sendbuf));
		memset(recvbuf,0,sizeof(recvbuf));
	}
	
	
	close(sock);
	return 0;
}
		