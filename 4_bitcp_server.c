// tcp_chat_server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 2036
#define BUFFER_SIZE 1024

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength;
    char buffer[BUFFER_SIZE];

    // 1. Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        printf("Socket creation failed\n");
        exit(1);
    }

    // 2. Initialize server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Binding failed\n");
        exit(1);
    }

    // 4. Listen for connections
    if (listen(serverSocket, 5) < 0)
    {
        printf("Listening failed\n");
        exit(1);
    }

    printf("Server is waiting for connection...\n");

    // 5. Accept connection
    clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, 
&clientAddressLength);
    if (clientSocket < 0)
    {
        printf("Accept failed\n");
        exit(1);
    }

    printf("Client connected successfully.\n");

    // 6. Bidirectional Chat
    while (1)
    {
        // Receive from client
        memset(buffer, 0, BUFFER_SIZE);
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        printf("Client: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("Client disconnected.\n");
            break;
        }

        // Send to client
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(clientSocket, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("Server disconnected.\n");
            break;
        }
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}
