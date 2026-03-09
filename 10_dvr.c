#include<stdio.h>      // Header file for input/output functions like printf and scanf
#include<stdlib.h>     // Standard library (not heavily used here but commonly included)

#define inf 999        // Define a constant 'inf' to represent infinity (no direct connection)

int main()
{
   int n,i,j,k;       // n = number of routers
                      // i = source router index
                      // j = destination router index
                      // k = intermediate router used to check shorter paths

   int cost[100][100],dist[100][100],next[100][100];
                      // cost[][] = stores direct cost between routers
                      // dist[][] = stores shortest distance between routers
                      // next[][] = stores next hop router to reach destination

   printf("Enter the number of routers:");
   scanf("%d",&n);    // Read number of routers in the network

   printf("Enter the cost matrix(999 for infinity)\n");
                      // User enters the cost between every pair of routers
                      // 999 means no direct path

   for(i=0;i<n;i++)   // Loop through each router (row of matrix)
   {
      for(j=0;j<n;j++)   // Loop through each destination router (column)
      {
         scanf("%d",&cost[i][j]);   // Read cost from router i to router j

         dist[i][j]=cost[i][j];     // Initially shortest distance equals direct cost

         if(i==j)                   // If source and destination routers are same
         {
            next[i][j]=i;           // Next router is itself
         }
         else if(cost[i][j]!=inf)   // If there is a direct connection
         {
            next[i][j]=j;           // Next hop is the destination router itself
         }
         else
         {
            next[i][j]=-1;          // If no connection exists, set next router as -1
         }
      }
   }

   for(k=0;k<n;k++)   // Outer loop: consider each router as an intermediate router
   {
      for(i=0;i<n;i++)   // Loop for source router
      {
         for(j=0;j<n;j++)   // Loop for destination router
         {
            // Check if path i → k → j is shorter than direct path i → j
            if(dist[i][j]>dist[i][k]+dist[k][j])
            {
               dist[i][j]=dist[i][k]+dist[k][j];
                        // Update shortest distance

               next[i][j]=next[i][k];
                        // Update next hop router
                        // To reach j from i, first go to the router used to reach k
            }
         }
      }
   }

   for(i=0;i<n;i++)   // Print routing table for each router
   {
      printf("\nRouter%d\n",i);    // Display router number

      printf("Destination Distance Next Router\n");
                                // Column headings

      for(j=0;j<n;j++)          // Loop through all destinations
      {
         printf("%d\t",j);      // Print destination router

         if(dist[i][j]>=inf)    // If distance is infinity (no path)
         {
            printf("-1\t");     // Show -1 for unreachable destination
         }
         else
         {
            printf("%d\t",dist[i][j]);   // Print shortest distance
         }

         printf("%d\n",next[i][j]);      // Print next router (next hop)
      }
   }

   printf("Cost Matrix\n  \t");   // Print final shortest path matrix

   for(i=0;i<n;i++)
   {
      printf("%d\t",i);           // Print column headers (router numbers)
   }

   printf("\n");

   for(i=0;i<n;i++)
   {
      printf("%d |\t",i);         // Print row header (router number)

      for(j=0;j<n;j++)
      {
         printf("%d\t",dist[i][j]);   // Print shortest distance from i to j
      }

      printf("\n");
   }

   return 0;   // End of program
}
