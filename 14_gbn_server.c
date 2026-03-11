#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>
#define port 1234

int main()
{
   int ssock,csock;            // Server and client socket descriptors
   struct sockaddr_in addr;
   socklen_t addrsize;
   int frame,ack,expected=0;   // expected = next expected frame number

   srand(time(NULL));           // Initialize random for simulating ACK loss

   // Create TCP socket
   ssock=socket(AF_INET,SOCK_STREAM,0);
   if(ssock<0)
   {
      printf("Error in creating server socket!\n");
      return 0;
   }
   printf("Server socket created successfully!\n");

   // Initialize server address
   memset(&addr,0,sizeof(addr));
   addr.sin_family=AF_INET;
   addr.sin_port=htons(port);
   addr.sin_addr.s_addr=inet_addr("127.0.0.1");

   bind(ssock,(struct sockaddr*)&addr,sizeof(addr));  // Bind socket to IP and port
   listen(ssock,5);                                   // Listen for incoming connections
   printf("Server listening\n");

   addrsize=sizeof(addr);
   csock=accept(ssock,(struct sockaddr*)&addr,&addrsize); // Accept client connection
   if(csock<0)
   {
      printf("Error in creating client socket!\n");
      return 0;
   }
   printf("Client socket created successfully!\n");

   while(1)
   {
      recv(csock,&frame,sizeof(frame),0);  // Receive frame from client

      if(frame==-1) // End of transmission
      {
         break;
      }

      printf("Received frame %d\n",frame);

      // Simulate random ACK loss
      if(frame==expected)
      {
         int loss=rand()%3; // 1/3 chance of ACK loss
         if(loss==0)
         {
            printf("ACK lost for frame %d\n",frame);
         }
         else
         {
            ack=frame;
            printf("Sending ACK %d\n",ack);
            send(csock,&ack,sizeof(ack),0); // Send ACK
            expected++; // Expect next frame
         }
      }
      else // Out-of-order frame
      {
         ack=expected-1;  // Send last correct ACK
         printf("Out of order.Sending last ack %d\n",ack);
         send(csock,&ack,sizeof(ack),0);
      }

      sleep(1);
   }

   close(csock);  // Close client socket
   close(ssock);  // Close server socket
   return 0;
}
