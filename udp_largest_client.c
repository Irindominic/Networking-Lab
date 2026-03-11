#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define port 10000
int main()
{
	int client_socket;
	struct sockaddr_in server_addr={AF_INET,htons(port),inet_addr("127.0.0.1")};
	socklen_t serveraddr_len=sizeof(server_addr);
	client_socket=socket(AF_INET,SOCK_DGRAM,0);
	int n;
	printf("Enter the size of array:");
	scanf("%d",&n);
	int numbers[n];
	printf("Enter the elements of array:");
	for(int i=0;i<n;i++)
	{
		scanf("%d",&numbers[i]);
	}
	sendto(client_socket,&n,sizeof(n),0,(struct sockaddr*)&server_addr,serveraddr_len);
	sendto(client_socket,numbers,sizeof(numbers),0,(struct sockaddr*)&server_addr,serveraddr_len);
	int largest;
recvfrom(client_socket,&largest,sizeof(largest),0,(struct sockaddr*)&server_addr,&serveraddr_len);
	printf("Largest num:%d\n",largest);
	close(client_socket);
	return 0;
}
