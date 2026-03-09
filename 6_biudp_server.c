
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

#define port 10240   // Port number for communication

int main()
{
    int server_sock;   // Socket descriptor for server

    socklen_t addr_size;   // Size of client address structure

    struct sockaddr_in server_addr,client_addr;  
    // server_addr → stores server IP and port
    // client_addr → stores client IP and port (received dynamically)

    char buff[1024];   // Buffer for storing messages

    // Creating UDP socket
    server_sock=socket(AF_INET,SOCK_DGRAM,0);
    // AF_INET → IPv4
    // SOCK_DGRAM → UDP protocol
    // 0 → default protocol

    if(server_sock<0)
    {
        printf("server not created\n");
        exit(0);
    }
    else
    {
        printf("Server created\n");
    }

    // Initialize server address structure with zero
    memset(&server_addr,'\0',sizeof(server_addr));

    server_addr.sin_family=AF_INET;   // IPv4 family

    server_addr.sin_port=htons(port); 
    // htons() → Host to Network Short conversion

    server_addr.sin_addr.s_addr=inet_addr("127.0.0.3");
    // Loopback IP address used for testing

    // Bind socket to IP address and port
    int n=bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr));

    if(n<0)
    {
        printf("Bind error\n");
        exit(0);
    }
    else
    {
        printf("Bind to port number %d\n",port);
        addr_size=sizeof(client_addr);  
        // Store client address structure size
    }

    // Infinite loop for continuous communication
    while(1)
    {
        bzero(buff,1024);  
        // Clear buffer before receiving message

        // Receive message from client
        recvfrom(server_sock,buff,sizeof(buff),0,
        (struct sockaddr*)&client_addr,&addr_size);

        printf("Client:%s",buff);  
        // Display client message

        // Exit condition check
        if(strncmp("exit",buff,4)==0)
        {
            printf("Disconnected\n");
            close(server_sock);   // Close socket
            break;
        }

        bzero(buff,1024);  
        // Clear buffer before sending message

        printf("Enter message:");
        fgets(buff,sizeof(buff),stdin);  
        // Take server user input

        // Send message to client
        sendto(server_sock,buff,strlen(buff),0,
        (struct sockaddr*)&client_addr,addr_size);

        // Exit condition check
        if(strncmp("exit",buff,4)==0)
        {
            printf("Disconnected\n");
            close(server_sock);   // Close socket
            break;
        }
    }

    return 0;
}
