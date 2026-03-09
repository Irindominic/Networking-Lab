#include <stdio.h>      // For printf()
#include <string.h>     // For strlen()
#include <unistd.h>     // For close()
#include <time.h>       // For time() and ctime()
#include <arpa/inet.h>  // For socket programming functions

int main() {

    int server_sock = socket(AF_INET, SOCK_DGRAM, 0);  
    // Create UDP socket
    // AF_INET → IPv4
    // SOCK_DGRAM → UDP protocol
    // 0 → default protocol

    struct sockaddr_in server_addr = {AF_INET, htons(12000), {INADDR_ANY}};
    // Server address structure initialization
    // AF_INET → IPv4
    // htons(12000) → convert port number to network byte order
    // INADDR_ANY → server accepts requests from any IP address

    bind(server_sock, (void*)&server_addr, sizeof(server_addr));
    // Bind socket to the specified IP address and port

    printf("Time Server started on port 12000\n");
    // Display server start message

    char buff[256];
    // Buffer to store the message received from the client

    while (1) {   // Infinite loop so server runs continuously

        struct sockaddr_in client_addr;
        // Structure to store client IP address and port

        socklen_t addr_size = sizeof(client_addr);
        // Variable to store size of client address structure

        int n = recvfrom(server_sock, buff, 255, 0, (void*)&client_addr, &addr_size);
        // Receive request from client
        // server_sock → server socket
        // buff → buffer to store received data
        // 255 → maximum bytes to receive
        // client_addr → stores client IP and port

        if (n <= 0) continue;
        // If no data or error occurs, skip and wait for next request

        buff[n] = 0;
        // Add null terminator to convert received data into a proper string

        printf("Time request from %s:%d\n", inet_ntoa(client_addr.sin_addr),
ntohs(client_addr.sin_port));
        // Display client IP and port number
        // inet_ntoa() → converts IP address to readable string
        // ntohs() → converts port number from network byte order to host byte order

        time_t now = time(NULL);
        // Get current system time in seconds since Jan 1, 1970

        char *timestr = ctime(&now);
        // Convert time into human-readable string format

        int len = strlen(timestr);
        // Calculate length of the time string

        sendto(server_sock, timestr, len, 0, (void*)&client_addr, addr_size);
        // Send the time string back to the client

        printf("Sent: %s", timestr);
        // Display the time sent to the client
    }

    close(server_sock);
    // Close the socket (this line normally won't execute because of infinite loop)
}
