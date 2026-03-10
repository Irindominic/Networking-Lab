#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12000   // Port number used for communication

// Frame structure used for both DATA frame and ACK frame
typedef struct {
    int seq_no, ack_no;   // seq_no → sequence number of frame
                          // ack_no → acknowledgment number sent back
    char data[1024];      // data payload of the frame
} Frame;

int main() {

    // server_sock → UDP socket descriptor
    // n → stores return value of recvfrom()
    // addr_size → size of client address structure
    // exp_seq → sequence number expected by receiver
    int server_sock, n, addr_size, exp_seq = 0;

    // server_addr → stores server IP and port
    // client_addr → stores client IP and port dynamically
    struct sockaddr_in server_addr, client_addr;

    // Frame variable used to receive DATA and send ACK
    Frame frame;

    // Create UDP socket
    if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket");   // prints error if socket creation fails
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure to zero
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;          // IPv4 address family
    server_addr.sin_addr.s_addr = INADDR_ANY;  // accept connection from any IP
    server_addr.sin_port = htons(PORT);   // convert port to network byte order

    // Bind socket with server address and port
    if (bind(server_sock, (struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("Bind");   // error if binding fails
        exit(EXIT_FAILURE);
    }

    printf("Receiver running on port %d...\n", PORT);

    // store size of client address
    addr_size = sizeof(client_addr);

    // infinite loop to continuously receive frames
    while (1) {

        // Receive frame from client
        n = recvfrom(server_sock, &frame, sizeof(Frame), 0,
                     (struct sockaddr *)&client_addr, &addr_size);

        // Simulate packet loss randomly
        // rand()%10 generates numbers 0–9
        // if value <3 → about 30% probability of frame loss
        if (rand() % 10 < 5) {
            printf("[Packet Loss] Dropped Frame %d\n", frame.seq_no);
            continue;   // frame ignored, no ACK sent
        }

        // If frame received successfully
        if (n > 0) {

            // Display received frame information
            printf("Received Frame: %d | Data: %s\n",frame.seq_no,frame.data);

            // Check if received frame sequence matches expected sequence
            if (frame.seq_no == exp_seq) {

                // Update next expected sequence number
                // Since Stop-and-Wait uses 1-bit sequence numbers
                // values alternate between 0 and 1
                exp_seq = 1 - exp_seq;

                // ACK number indicates the next expected frame
                frame.ack_no = exp_seq;

                printf("  -> Frame accepted. Sending ACK %d\n",frame.ack_no);

                // seq_no is not needed in ACK frame
                frame.seq_no = 0;

            } 
            else {

                // If sequence number is not expected
                // it means duplicate frame was received
                // resend ACK of next expected frame
                frame.ack_no = exp_seq;

                printf("  -> Duplicate! Resending ACK %d\n",frame.ack_no);
            }

            // Send ACK frame back to client
            sendto(server_sock, &frame, sizeof(Frame), 0,
                   (struct sockaddr *)&client_addr, addr_size);
        }
    }

    // Close server socket (not reached due to infinite loop)
    close(server_sock);

    return 0;
}
