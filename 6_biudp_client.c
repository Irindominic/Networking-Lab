#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

#define port 10240   // Communication port number

int main()
{
    int client_socket;   // UDP client socket descriptor

    struct sockaddr_in server_address;
    // Stores server IP address and port number

    socklen_t address_length;

    char message_buffer[1024];   // Buffer for storing messages

    // Create UDP socket (Connectionless communication)
    client_socket=socket(AF_INET,SOCK_DGRAM,0);

    if(client_socket<0)
    {
        printf("client is not created\n");
        exit(0);
    }
    else
    {
        printf("client is created successfully\n");
    }

    // Initialize server address structure memory
    memset(&server_address,'\0',sizeof(server_address));

    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(port);

    // Server IP address (Loopback testing address)
    server_address.sin_addr.s_addr=inet_addr("127.0.0.3");

    address_length=sizeof(server_address);

    // Continuous communication loop
    while(1)
    {
        bzero(message_buffer,1024);

        printf("Enter the message-");

        // Read user input message
        fgets(message_buffer,sizeof(message_buffer),stdin);

        // Send message to server
        sendto(client_socket,
               message_buffer,
               strlen(message_buffer),
               0,
               (struct sockaddr*)&server_address,
               address_length);

        // Exit condition check
        if(strncmp("exit",message_buffer,4)==0)
        {
            printf("Disconnected");
            close(client_socket);
            break;
        }

        bzero(message_buffer,1024);

        // Receive message from server
        recvfrom(client_socket,
                 message_buffer,
                 sizeof(message_buffer),
                 0,
                 (struct sockaddr*)&server_address,
                 &address_length);

        printf("Server:%s",message_buffer);

        // Exit condition check
        if(strncmp("exit",message_buffer,4)==0)
        {
            printf("Disconnected");
            close(client_socket);
            break;
        }
    }

    return 0;
}
