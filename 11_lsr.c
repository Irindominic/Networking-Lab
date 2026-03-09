#include<stdio.h>   // Standard input-output library for printf and scanf

int main()
{
	int n,i,j,k,v,min; 
	// n  -> number of routers
	// i,j -> loop variables
	// k   -> declared but not used in this program
	// v   -> router with the current minimum distance
	// min -> stores the minimum distance during comparison

	int adj[100][100],dist[100],last[100];
	// adj  -> adjacency matrix storing distance between routers
	// dist -> shortest distance from source router to every other router
	// last -> stores previous router in the shortest path

	int flag[100],src;
	// flag -> indicates whether a router is visited (1) or not visited (0)
	// src  -> source router

	printf("Enter the no.of routers:\n");
	scanf("%d",&n);   // user enters number of routers

	printf("Enter the distance matrix:\n");

	for(i=0;i<n;i++)   // loop through rows of matrix
	{
		for(j=0;j<n;j++)   // loop through columns of matrix
		{
			scanf("%d",&adj[i][j]);   // read distance between router i and j

			// if distance is 0 and routers are different,
			// it means there is no direct connection
			// so replace it with a large value (infinity)
			if(adj[i][j]==0 && i!=j)
			{
				adj[i][j]=1000;
			}
		}
	}

	printf("Enter the source router:\n");
	scanf("%d",&src);   // user enters source router

	for(i=0;i<n;i++)
	{
		flag[i]=0;            // initially mark all routers as unvisited
		dist[i]=adj[src][i];  // initialize distance from source to router i
		last[i]=src;          // assume source is the previous router initially
	}

	flag[src]=1;   // mark source router as visited
	dist[src]=0;   // distance from source to itself is 0

	for(i=0;i<n;i++)   // main loop of Dijkstra's algorithm
	{
		min=1000;   // initialize minimum distance as large value

		for(j=0;j<n;j++)
		{
			// find the unvisited router with the smallest distance
			if(flag[j]==0&& dist[j]<min)
			{
				min=dist[j];   // update minimum distance
				v=j;           // store router index with minimum distance
			}
		}

		flag[v]=1;   // mark that router as visited

		for(j=0;j<n;j++)
		{
			if(flag[j]==0)   // check only unvisited routers
			{
				// check if going through router v gives a shorter path
				if(dist[v]+adj[v][j]<dist[j])
				{
					dist[j]=dist[v]+adj[v][j];  // update shortest distance
					last[j]=v;                  // update previous router
				}
			}
		}
	}

	for(i=0;i<n;i++)
	{
		// print shortest distance from source router to router i
		printf("Shortest path cost from %d to %d is %d\n",src,i,dist[i]);
	}

	return 0;   // program ends successfully
}
