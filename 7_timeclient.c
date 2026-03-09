#include <stdio.h>      // For input/output functions like printf(), puts()
#include <string.h>     // For string handling functions like strlen()
#include <unistd.h>     // For close() function
#include <arpa/inet.h>  // For socket structures and functions like inet_pton()

int main(int arg_count, char **arg_values) 
{    // Program starts here. arg_count = number of arguments, arg_values = argument list

    if (arg_count < 2) {                       // Check if server IP address is provided
        puts("Usage: ./time_client <server-ip>"); // Print correct usage if argument missing
        return 1;                              // Exit program with error code
    }

    int client_sock = socket(AF_INET, SOCK_DGRAM, 0); 
    // Create a socket
    // AF_INET → IPv4 addressing
    // SOCK_DGRAM → UDP protocol (connectionless communication)
    // 0 → Let OS choose default protocol for UDP

    struct sockaddr_in server_addr = {AF_INET, htons(12000)};
    // Define server address structure
    // AF_INET → IPv4 address family
    // htons(12000) → Convert port 12000 from host byte order to network byte order

    inet_pton(AF_INET, arg_values[1], &server_addr.sin_addr);
    // Convert the server IP address from text format to binary format
    // AF_INET → IPv4
    // arg_values[1] → server IP entered in command line
    // &server_addr.sin_addr → location where converted IP is stored

    char *request_msg = "TIME";
    // Message sent to the server requesting the current time

    sendto(client_sock, request_msg, strlen(request_msg), 0,
           (void*)&server_addr, sizeof(server_addr));
    // Send UDP datagram to the server
    // client_sock → socket descriptor
    // request_msg → message to send
    // strlen(request_msg) → length of message
    // 0 → flags (not used)
    // server_addr → destination server address
    // sizeof(server_addr) → size of address structure

    printf("Time request sent to %s:12000\n", arg_values[1]);
    // Display message indicating request was sent to server

    char buff[256];
    // Buffer to store time received from server

    struct sockaddr_in from_addr;
    // Structure to store address of sender (server)

    socklen_t from_len = sizeof(from_addr);
    // Variable to store size of sender address

    int n = recvfrom(client_sock, buff, 255, 0, (void*)&from_addr, &from_len);
    // Receive message from server
    // client_sock → socket descriptor
    // buff → buffer to store received message
    // 255 → maximum bytes to receive
    // 0 → flags
    // from_addr → store sender address
    // from_len → size of sender address

    if (n > 0) {               // If data is successfully received
        buff[n] = 0;           // Add null terminator to make it a valid string
        printf("Server Time: %s", buff);  // Display time received from server
    }

    close(client_sock);        // Close the UDP socket

}
