#include <stdio.h>      // For printf()
#include <stdlib.h>     // For exit()
#include <string.h>     // For strlen(), snprintf()
#include <unistd.h>     // For fork(), close(), getpid()
#include <arpa/inet.h>  // For socket programming functions

int main() {
    int serverSocket,clientSocket, n;// serverSocket:listening socket,clientSocket:communication socket
    struct sockaddr_in serverAddress = {AF_INET, htons(10121), INADDR_ANY}; // Server IP and port configuration
    struct sockaddr_in clientAddress;// To store client's address when connection is accepted
    socklen_t clientAddressLength = sizeof(clientAddress); // Size of client address structure
    char buffer[4096], fname[256], content[2048]; // Buffers for sending data,filename,and file content

    // Create a TCP socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        exit(1); // Exit if socket creation fails

    // Bind the socket to the server address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) 
        exit(1); // Exit if bind fails

    // Start listening for incoming connections (max queue size = 5)
    listen(serverSocket, 5);

    // Display server PID and that it is listening
    printf("[Main Server PID: %d] Listening on port 8080...\n", getpid());

    // Server runs forever accepting clients
    while (1) {

        // Accept a client connection
     if((clientSocket=accept(serverSocket,(struct sockaddr*)&clientAddress,&clientAddressLength)) < 0) 
            continue; // If accept fails, skip and wait for next client

        printf("[Main Server] Connection accepted.\n");

        // Create a new process for handling the client
        if (fork() == 0) 
        { 
            // Child Process

            // Child does not need the listening socket
            close(serverSocket);

            // Receive filename from client
            n = recv(clientSocket, fname, 255, 0); 
            fname[n] = 0; // Add null terminator to make it a valid string

            // Print requested file name
            printf("[Child PID: %d] Requested file: %s\n", getpid(), fname);

            // Try opening the requested file
            FILE *fp = fopen(fname, "r");

            if (fp) 
            {
                // If file exists, read its content
                n = fread(content, 1, 2047, fp); 
                content[n] = 0; // Null terminate content
                fclose(fp); // Close file after reading

                // Prepare response message containing server PID and file content
            snprintf(buffer,sizeof(buffer),"\n[Server PID: %d\n File Content:\n%s\n",getpid(),content);
            } 
            else
            {
                // If file not found, prepare error message
snprintf(buffer,sizeof(buffer),"\n[Server PID: %d]\nError:File '%s'not found.\n",getpid(),fname);
            }

            // Send response to client
            send(clientSocket, buffer, strlen(buffer), 0);

            // Print status message
            printf("[Child PID: %d] Response sent. Closing connection.\n", getpid());

            // Close client socket after communication
            close(clientSocket);

            // Terminate child process
            exit(0);
        }

        // Parent process closes client socket because child handles communication
        close(clientSocket);
    }

    return 0;
}
