#include<stdio.h>      // For printf()
#include<stdlib.h>     // For general functions
#include<string.h>     // For memset()
#include<unistd.h>     // For close(), sleep()
#include<arpa/inet.h>  // For socket programming functions
#include<time.h>       // For time related functions
#define port 1234      // Port number used for connection

int main()
{
    int csock;                     // Client socket descriptor
    struct sockaddr_in addr;       // Structure to store server address information
    socklen_t addrsize;            // Variable to store size of address structure
    int base=0, next=0, ack;       // base = first frame in window
                                   // next = next frame to send
                                   // ack = acknowledgement received from server
    int sent[8] = {0};             // Array to track status of frames in window
                                   // 0 = not sent / acknowledged
                                   // 1 = sent but not yet acknowledged

    // Create client socket
    csock = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation failed
    if(csock < 0) {
        printf("Error in creating client socket!\n");
        return 0;
    }

    printf("Client socket created successfully!\n");

    // Initialize address structure with zeros
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;                  // IPv4 address family
    addr.sin_port = htons(port);                // Convert port number to network byte order
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address (localhost)

    // Establish connection with server
    connect(csock, (struct sockaddr*)&addr, sizeof(addr));

    // Continue sending frames until all 10 frames are acknowledged
    while(base < 10)
    {
        // Send all frames inside the sliding window
        while(next < base + 4 && next < 10)
        {
            // Check if frame was not sent before
            if(sent[next % 8] == 0) {

                // Print frame number being sent
                printf("Sending frame:%d\n", next);

                // Send frame number to server
                send(csock, &next, sizeof(next), 0);

                // Mark frame as sent
                sent[next % 8] = 1;
            }

            // Move to next frame
            next++;
        }

        // Prepare file descriptor set for select()
        fd_set readfds;

        // Structure to store timeout value
        struct timeval tv;

        // Clear the file descriptor set
        FD_ZERO(&readfds);

        // Add client socket to read set
        FD_SET(csock, &readfds);

        // Timeout = 3 seconds
        tv.tv_sec = 3;
        tv.tv_usec = 0;

        // Wait for ACK from server or timeout
        int activity = select(csock + 1, &readfds, NULL, NULL, &tv);

        // If activity > 0, data (ACK/NAK) received
        if(activity > 0)
        {
            // Receive acknowledgement
            recv(csock, &ack, sizeof(ack), 0);

            // If ACK is positive → normal acknowledgement
            if(ack >= 0) {

                printf("Received ACK %d\n", ack);

                // Mark frame as acknowledged
                sent[ack % 8] = 0;

                // Slide window forward
                while(sent[base % 8] == 0 && base < next)
                    base++;
            } 
            else {

                // Negative ACK means frame lost
                int lost_frame = -ack;

                printf("Received NAK for frame %d\n", lost_frame);

                printf("Resending frame %d due to NAK\n", lost_frame);

                // Resend the lost frame
                send(csock, &lost_frame, sizeof(lost_frame), 0);
            }
        }
        else
        {
            // If select() timed out → ACK not received
            printf("Timeout...Resending frames in window\n");

            // Resend all unacknowledged frames in the window
            for(int i = base; i < next; i++)
            {
                if(sent[i % 8] == 1)
                {
                    printf("Resending frame %d\n", i);

                    send(csock, &i, sizeof(i), 0);
                }
            }
        }

        // Small delay to slow down sending
        sleep(1);
    }

    // Send termination signal to server
    int end = -1;

    send(csock, &end, sizeof(end), 0);

    // Close client socket
    close(csock);

    return 0;
}
