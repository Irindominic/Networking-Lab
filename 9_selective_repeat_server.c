// Selective Repeat Server Program

#include<stdio.h>      // for printf(), scanf()
#include<stdlib.h>     // for rand(), srand()
#include<string.h>     // for memset()
#include<unistd.h>     // for close()
#include<arpa/inet.h>  // for socket programming functions
#include<time.h>       // for time() used in random seed

#define port 1234      // Port number used by server

int main()
{
    int ssock, csock;                 // ssock = server socket, csock = client socket
    struct sockaddr_in addr;          // structure to store IP address and port
    socklen_t addrsize;               // variable to store size of address structure
    int frame, ack;                   // frame = received frame number, ack = acknowledgement
    int received[10] = {0};           // array to track which frames have been received

    srand(time(NULL));                // initialize random number generator

    // Create server socket
    ssock = socket(AF_INET, SOCK_STREAM, 0);

    if(ssock < 0) {
        printf("Error in creating server socket!\n");
        return 0;
    }

    printf("Server socket created successfully!\n");

    // Initialize address structure with zeros
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;             // IPv4 address family
    addr.sin_port = htons(port);           // convert port number to network byte order
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // loopback IP address (localhost)

    // Bind server socket to the specified IP and port
    bind(ssock, (struct sockaddr*)&addr, sizeof(addr));

    // Put server into listening mode
    listen(ssock, 5);

    printf("Server listening!\n");

    // Accept connection request from client
    addrsize = sizeof(addr);
    csock = accept(ssock, (struct sockaddr*)&addr, &addrsize);

    // Infinite loop to receive frames continuously
    while(1)
    {
        // Receive frame number from client
        recv(csock, &frame, sizeof(frame), 0);

        // If client sends -1, terminate the communication
        if(frame == -1) break;

        printf("Received frame %d\n", frame);

        // Generate random number to simulate frame loss
        int loss = rand() % 4;

        // If random value becomes 0, simulate frame loss
        if(loss == 0) {
            printf("Frame %d lost. Sending NAK %d\n", frame, frame);

            // Send NAK as negative value of frame number
            int nak = -frame;

            send(csock, &nak, sizeof(nak), 0);

            // Skip further processing and continue loop
            continue;
        }

        // Check whether frame was already received
        if(received[frame] == 0) {

            // Mark frame as received
            received[frame] = 1;

            printf("Frame %d accepted\n", frame);

        } 
        else {

            // If already received earlier, it is a duplicate frame
            printf("Duplicate frame!\n");

        }

        // Prepare acknowledgement for the received frame
        ack = frame;

        printf("Sending ACK %d\n", ack);

        // Send acknowledgement to client
        send(csock, &ack, sizeof(ack), 0);
    }

    // Close client socket
    close(csock);

    // Close server socket
    close(ssock);

    return 0;
}
