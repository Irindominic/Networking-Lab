#include <stdio.h>      // For printf(), perror()
#include <stdlib.h>     // For exit()
#include <string.h>     // For strlen(), memset(), strcspn()
#include <unistd.h>     // For close()
#include <pthread.h>    // For multithreading
#include <arpa/inet.h>  // For socket programming functions

#define BUFFER_SIZE 1024   // Size of message buffer

int sockfd;   // Global socket file descriptor (used by both threads)


// Thread function to receive messages from server
void* receive_messages(void* arg) {
    char buffer[BUFFER_SIZE];   // Buffer to store incoming messages

    while (1) {   // Infinite loop to continuously receive messages
        memset(buffer, 0, BUFFER_SIZE);   // Clear buffer before receiving

        // Receive message from server
        int n = recv(sockfd, buffer, BUFFER_SIZE, 0);

        // If server disconnected or error occurred
        if (n <= 0) {
            printf("\nServer disconnected.\n");
            close(sockfd);   // Close socket
            exit(0);         // Terminate entire program
        }

        // Print received message
        printf("\nMessage: %s\n> ", buffer);

        // Force immediate output display
        fflush(stdout);
    }

    return NULL;   // Thread must return void*
}


int main() {
    struct sockaddr_in server_addr;   // Structure to store server address details

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {   // Check if socket creation failed
        perror("Socket creation failed");
        exit(1);
    }

    // Set server address family (IPv4)
    server_addr.sin_family = AF_INET;

    // Set port number (convert to network byte order)
    server_addr.sin_port = htons(8090);

    // Convert IP address from text to binary form
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr,
sizeof(server_addr)) < 0) {

        perror("Connection failed");
        exit(1);
     }

    printf("Connected to chat server.\n");

    pthread_t tid;   // Thread ID variable

    // Create new thread for receiving messages
    pthread_create(&tid, NULL, receive_messages, NULL);

    // Detach thread (automatic cleanup after termination)
    pthread_detach(tid);

    char message[BUFFER_SIZE];   // Buffer for sending messages

    while (1) {   // Infinite loop for sending messages
        printf("> ");   // Display prompt

        // Read input from user
        fgets(message, BUFFER_SIZE, stdin);

        // Remove newline character added by fgets()
        message[strcspn(message, "\n")] = 0;

        // Send message to server
        send(sockfd, message, strlen(message), 0);
    }

    close(sockfd);   // Close socket (never reached due to infinite loop)

    return 0;   // Return success
}
