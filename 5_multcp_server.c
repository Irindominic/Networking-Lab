#include <stdio.h>      // For printf(), perror()
#include <stdlib.h>     // For malloc(), free(), exit()
#include <string.h>     // For strlen(), memset()
#include <unistd.h>     // For close()
#include <pthread.h>    // For multithreading (pthread functions)
#include <arpa/inet.h>  // For socket programming functions and structures

#define MAX_CLIENTS 10      // Maximum number of clients allowed
#define BUFFER_SIZE 1024    // Size of message buffer

int client_sockets[MAX_CLIENTS] = {0};   // Array to store connected client socket file descriptors
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // Mutex for synchronizing shared data access

// Function to send message to all clients except sender
void broadcast_message(char *msg, int sender_fd) {
    pthread_mutex_lock(&lock);   // Lock before accessing shared client_sockets array

    for (int i = 0; i < MAX_CLIENTS; i++) {
        // Check if slot is not empty and not the sender
        if (client_sockets[i] != 0 && client_sockets[i] != sender_fd) {
            send(client_sockets[i], msg, strlen(msg), 0);  // Send message
        }
    }

    pthread_mutex_unlock(&lock); // Unlock after finishing
}

// Thread function to handle each client
void* handle_client(void* arg) {
    int client_fd = *(int*)arg;  // Extract client socket from argument
    free(arg);                  // Free dynamically allocated memory

    char buffer[BUFFER_SIZE];   // Buffer to store incoming message

    while (1) 
    {
        memset(buffer, 0, BUFFER_SIZE);  // Clear buffer before receiving
        int n = recv(client_fd, buffer, BUFFER_SIZE, 0);  // Receive message from client

        if (n <= 0)   // If client disconnected or error occurred
	{
            printf("Client disconnected\n");
            close(client_fd);  // Close client socket

            // Remove client from client_sockets array
            pthread_mutex_lock(&lock);
            for (int i = 0; i < MAX_CLIENTS; i++) 
	    {
                if (client_sockets[i] == client_fd) 
		{
                    client_sockets[i] = 0;  // Mark slot as empty
                    break;
                }
            }
            pthread_mutex_unlock(&lock);
            break;  // Exit loop and end thread
        }

        printf("Client: %s\n", buffer);  // Print received message
        broadcast_message(buffer, client_fd);  // Send message to other clients
    }

    return NULL;  // Thread must return void*
}

int main() {
    int server_fd;   // Server socket file descriptor
    struct sockaddr_in server_addr, client_addr;  // Address structures
    socklen_t addr_len = sizeof(client_addr);     // Size of client address

    server_fd = socket(AF_INET, SOCK_STREAM, 0);  // Create TCP socket
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    int opt = 1;
    // Allow reuse of address (prevents "Address already in use" error)
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;         // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    server_addr.sin_port = htons(8090);      // Port number (converted to network byte order)

    // Bind socket to IP and port
    if (bind(server_fd, (struct sockaddr*)&server_addr,
sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server listening on port ...\n");

    while (1) {
        // Accept new client connection
        int client_fd = accept(server_fd, (struct
sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;   // Skip this iteration if accept fails
        }

        pthread_mutex_lock(&lock);  // Lock before modifying shared array
        int i;
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {  // Find empty slot
                client_sockets[i] = client_fd;  // Store client socket
                break;
            }
        }
        pthread_mutex_unlock(&lock);

        if (i == MAX_CLIENTS) {
            // If no empty slot found
            printf("Max clients reached. Connection rejected.\n");
            close(client_fd);
        } else {
            printf("New client connected.\n");

            int *new_sock = malloc(sizeof(int));  // Allocate memory for socket
            *new_sock = client_fd;                // Store client socket

            pthread_t tid;
            // Create new thread to handle client
            pthread_create(&tid, NULL, handle_client, new_sock);
            pthread_detach(tid);  // Detach thread (auto cleanup)
        }
    }

    close(server_fd);  // Close server socket (never reached due to infinite loop)
    return 0;
}
