#include <stdio.h>      // For printf() and scanf()
#include <stdlib.h>     // Standard library (not strictly needed here but commonly included)

int main()
{
    // Variable declarations
    int bucket_size, output_rate, n, i, dropped;
    // bucket_size  -> Maximum capacity of the bucket
    // output_rate  -> Rate at which packets leave the bucket
    // n            -> Number of incoming packets
    // i            -> Loop counter (also used as time)
    // dropped      -> Amount of packet dropped due to overflow

    int packets[100], bucket_content = 0;
    // packets[100] -> Array storing packet sizes
    // bucket_content -> Current amount of data in bucket (initially empty)

    printf("Enter the bucket size: ");
    scanf("%d", &bucket_size);   // Read maximum bucket capacity

    printf("Enter the output rate: ");
    scanf("%d", &output_rate);   // Read constant rate of packet transmission

    printf("Enter the number of inputs: ");
    scanf("%d", &n);             // Read number of packets arriving

    printf("Enter the packet sizes:\n");

    // Loop to read packet sizes
    for(i = 0; i < n; i++)
    {
        scanf("%d", &packets[i]);  // Store each packet size in the array
    }

    // Print table heading
    printf("\nTime\tPacket Size\tBucket Content\tOutput\tDropped\tRemaining\n");

    // MAIN SIMULATION LOOP (Packet arrival phase)
    for(i = 0; i < n; i++)
    {
        dropped = 0;   // Reset dropped value for every time unit

        // Check if packet can fit into the bucket
        if((bucket_content + packets[i]) <= bucket_size)
        {
            // If it fits, add packet to bucket
            bucket_content += packets[i];
        }
        else
        {
            // Overflow condition
            // Calculate how much exceeds bucket capacity
            dropped = (bucket_content + packets[i]) - bucket_size;

            // Bucket becomes full
            bucket_content = bucket_size;
        }

        // Now simulate packet transmission (leakage)

        if(bucket_content < output_rate)
        {
            // If bucket has less data than output rate
            // Send all available data

            printf("%d\t%d\t\t%d\t\t%d\t%d\t%d\n",
                   i+1, packets[i], bucket_content,
                   bucket_content, dropped, 0);

            // Bucket becomes empty after sending
            bucket_content = 0;
        }
        else
        {
            // If bucket has enough data
            // Send only the fixed output rate

            printf("%d\t%d\t\t%d\t\t%d\t%d\t%d\n",
                   i+1, packets[i], bucket_content,
                   output_rate, dropped,
                   bucket_content - output_rate);

            // Reduce bucket content after transmission
            bucket_content -= output_rate;
        }
    }

    // CONTINUE LEAKING AFTER ALL PACKETS ARRIVE
    // This loop runs until bucket becomes empty
    while(bucket_content > 0)
    {  
        if(bucket_content < output_rate)
        {
            // Send remaining data if it is less than output rate

            printf("%d\t%d\t\t%d\t\t%d\t%d\t%d\n",
                   ++i, 0, bucket_content,
                   bucket_content, dropped, 0);

            // Bucket becomes empty
            bucket_content = 0;
        }
        else
        {
            // Send fixed output rate

            printf("%d\t%d\t\t%d\t\t%d\t%d\t%d\n",
                   ++i, 0, bucket_content,
                   output_rate, dropped,
                   bucket_content - output_rate);

            // Reduce bucket content
            bucket_content -= output_rate;
        }
    }

    return 0;   // Program finished successfully
}
