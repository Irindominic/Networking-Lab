#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#define port 10000
int main()
{
	int server_socket;
	struct sockaddr_in server_addr={AF_INET,htons(port),inet_addr("127.0.0.1")};
	struct sockaddr_in client_addr;
	socklen_t clientaddr_len=sizeof(client_addr);
	server_socket=socket(AF_INET,SOCK_DGRAM,0);
	if(server_socket<0)
	{
		printf("Socket not created\n");
		exit(1);
	}
	if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		printf("Errro in binding\n");
		exit(1);
	}
	printf("Bind to port number %d\n",port);
	int n;
	recvfrom(server_socket,&n,sizeof(n),0,(struct sockaddr*)&client_addr,&clientaddr_len);
	int numbers[n];
	recvfrom(server_socket,numbers,sizeof(numbers),0,(struct sockaddr*)&client_addr,&clientaddr_len);
	int largest=numbers[0];
	printf("%d\n",numbers[0]);
	for(int i=1;i<n;i++)
	{
		if(numbers[i]>largest)
		{
			largest=numbers[i];
		}
	}
	printf("%d\n",largest);
	sendto(server_socket,&largest,sizeof(largest),0,(struct sockaddr*)&client_addr,clientaddr_len);	
	close(server_socket);
	return 0;
}
