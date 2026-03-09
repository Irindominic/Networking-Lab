#include <stdio.h>      // For printf(), scanf()
#include <stdlib.h>     // For exit()
#include <string.h>     // For strlen()
#include <unistd.h>     // For close()
#include <arpa/inet.h>  // For socket functions and sockaddr_in structure

int main() {

    int clientSocket;   // Socket used by the client to communicate with the server

    // Structure to store server IP address and port
    struct sockaddr_in serverAddress = {AF_INET, htons(10121), inet_addr("127.0.0.1")};

    char fname[256],buf[2048];//fname stores the filename entered by user,buf storesserver response

    // Create a TCP socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit(1); // Exit if socket creation fails

    // Connect to the server using server IP and port
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        exit(1); // Exit if connection fails

    // Ask user for filename
    printf("Enter filename to request: ");

    // Read filename from user
    scanf("%s", fname);

    // Send filename to the server
    send(clientSocket, fname, strlen(fname), 0);

    // Receive response from server (file content or error message)
    if (recv(clientSocket, buf, 2048, 0) > 0)
        printf("Server Response:\n%s\n", buf);  // Print server response
    else
        printf("Receive failed\n"); // If recv fails

    // Close the client socket after communication
    close(clientSocket);

    return 0;
}
