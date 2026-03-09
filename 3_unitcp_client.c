// uni_tcp_client.c
// Unidirectional TCP Communication
// Client sends messages to server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        printf("Client socket creation failed\n");
        exit(1);
    }

    // Initialize server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Connection failed\n");
        exit(1);
    }

    printf("Connected to server successfully\n");

    // Send messages to server
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        send(clientSocket, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "exit", 4) == 0)
        {
            printf("Client disconnected\n");
            break;
        }
    }

    // Close socket after communication ends
    close(clientSocket);

    return 0;
}
