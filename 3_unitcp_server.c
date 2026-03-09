// uni_tcp_server.c
// Unidirectional TCP Communication
// Server receives messages from client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength;
    char buffer[BUFFER_SIZE];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        printf("Server socket creation failed\n");
        exit(1);
    }

    printf("Server socket created successfully\n");

    // Initialize server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind socket to IP and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Binding failed\n");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0)
    {
        printf("Listening failed\n");
        exit(1);
    }

    printf("Waiting for client connection...\n");

    // Accept client connection
    clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket < 0)
    {
        printf("Accept failed\n");
        exit(1);
    }

    printf("Client connected successfully\n");

    // Receive messages from client
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        printf("Message from Client: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("Client disconnected\n");
            break;
        }
    }

    // Close sockets after communication ends
    close(clientSocket);
    close(serverSocket);

    return 0;
}
