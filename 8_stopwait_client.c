#include <stdio.h>      // printf(), sprintf()
#include <stdlib.h>     // exit()
#include <string.h>     // memset()
#include <unistd.h>     // sleep(), close()
#include <arpa/inet.h>  // socket programming structures
#include <sys/time.h>   // struct timeval for timeout

#define PORT 12000   // Port number used to communicate with server

// Frame structure used to send DATA and receive ACK
typedef struct {
    int seq_no, ack_no;// seq_no → sequence number of frame
                     // ack_no → acknowledgment number sent by receiver
    char data[1024];      // actual data payload
} Frame;

int main() {

    // client_socket → UDP socket descriptor
    // i → packet counter (how many packets sent)
    // seq → sequence number used in Stop-and-Wait (0 or 1)
    // n → stores return value of recvfrom()
    // address_length → size of server address structure
    int client_socket, i = 0, seq = 0, n, address_length;

    // Structure to store server IP and port
    struct sockaddr_in server_address;

    // Frame variable used to send data frames and receive ACK frames
    Frame frame;

    // Timeout structure used for Stop-and-Wait retransmission
    // {2,0} means wait 2 seconds for ACK
    struct timeval tv = {2, 0};

    // Create UDP socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    { 
        perror("Socket"); // Print error message if socket creation fails
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure with zeros
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;   // IPv4 address family
    server_address.sin_port = htons(PORT);//Convert port number to network byte order
    server_address.sin_addr.s_addr = INADDR_ANY;//Server IP address

    // Set timeout for receiving ACK
    // If ACK not received within 2 seconds → recvfrom() returns error
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    // Send 5 packets
    while (i < 5) {

        printf("------------------------------------------\n");

        // Assign sequence number to frame
        frame.seq_no = seq;

        // Create data packet message
        sprintf(frame.data, "Packet %d", i);

        // Display frame being sent
        printf("Sending Frame %d: %s\n", seq, frame.data);

        // Send frame to receiver
sendto(client_socket,&frame,sizeof(Frame),0,(struct sockaddr *)&server_address,sizeof(server_address));

        // Store address structure size
        address_length = sizeof(server_address);

        // Wait for ACK from receiver
n=recvfrom(client_socket,&frame,sizeof(Frame),0,(struct sockaddr *)&server_address,&address_length);

        // If recvfrom() returns <0 → timeout occurred
        if (n < 0)
            printf("!! TIMEOUT !! Retransmitting...\n");

        // If ACK received successfully
        else if (frame.ack_no!= seq) {

            // Correct ACK received
            printf("ACK %d received.Move to next packet.\n",frame.ack_no);

            // Toggle sequence number (0 → 1 or 1 → 0)
            seq = 1 - seq;

            // Increase packet counter
            i++;
        }

        // Small delay for clear output observation
        sleep(1);
    }

    // Close the socket after sending all packets
    close(client_socket);

    return 0;
}
