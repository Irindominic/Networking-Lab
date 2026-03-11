#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/select.h>
#define port 1234

int main()
{
   int csock;                // Client socket descriptor
   struct sockaddr_in addr;  // Server address structure
   socklen_t addrsize;
   int base=0, next=0, ack;//base=first unACKed frame,next= next frame to send, ack=received ACK

   // Create TCP socket
   csock=socket(AF_INET,SOCK_STREAM,0);
   if(csock<0)
   {
      printf("Error in creating client socket!\n");
      return 0;
   }
   printf("Client socket created successfully!\n");

   // Initialize server address
   memset(&addr,0,sizeof(addr));
   addr.sin_family=AF_INET;
   addr.sin_port=htons(port);          // Server port
   addr.sin_addr.s_addr=inet_addr("127.0.0.1"); // Server IP

   // Connect to server
   connect(csock,(struct sockaddr*)&addr,sizeof(addr));

   // Send frames 0 to 4
   while(base<5)
   {
      // Send all frames in the window (window size = 4)
      while(next<4+base && next<5)
      {
         printf("Sending frame %d\n",next);
         send(csock,&next,sizeof(next),0);  // Send frame number
         next++;
      }

      // Wait for ACK using select() for timeout
      fd_set readfds;
      struct timeval tv;
      FD_ZERO(&readfds);
      FD_SET(csock,&readfds);
      tv.tv_sec=3;    // Timeout = 3 sec
      tv.tv_usec=0;

      int activity=select(csock+1,&readfds,NULL,NULL,&tv); // Wait for ACK or timeout

      if(activity>0) // ACK received
      {
         recv(csock,&ack,sizeof(ack),0);  // Receive ACK
         printf("Received ACK %d\n",ack);
         base=ack+1;  // Slide window
      }
      else // Timeout occurred
      {
         printf("Timeout.Resending frame %d\n",base);
         next=base;   // Go back and resend from first unACKed frame
      }
      sleep(1);
   }

   int end=-1;       // Indicate end of transmission
   send(csock,&end,sizeof(end),0);
   close(csock);     // Close socket
   return 0;
}
