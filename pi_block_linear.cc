#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <algorithm>

using namespace std;

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    long long int range = tosses / world_size , begin_idx = 0,begin,end; 
    static unsigned int seed = time(NULL) + world_rank;
    long long int x,y;
    double distance_squared;
    begin = range * world_rank;
    end = min(range * (world_rank + 1) , tosses);
    long long int count = 0;
    int dst = 0;

    // TODO: init MPI

    srand(seed);
    // TODO: handle workers
    for(int i = begin ; i < end ; i++){
        x = rand();
        y = rand();
        distance_squared = x * x + y * y;
        printf("%lf ",distance_squared);
        if ( distance_squared <= 1)
            count++;
    }

    if (world_rank > 0)
    {
        MPI_Send(&count ,1 ,MPI_LONG_LONG,dst ,0 ,MPI_COMM_WORLD );
    }
    else if (world_rank == 0)
    {
        MPI_Status status; 
        long long int temp;
        for(int source = 1 ; source < world_size ; source++){
            MPI_Recv(&temp, 1, MPI_LONG_LONG, source, 0, MPI_COMM_WORLD, &status);
            printf("%lld\n",temp);
            count += temp;
        }
        // TODO: master
    }

    if (world_rank == 0)
    {
        // TODO: process PI result
        double pi_estimate = 4 * count /(( double ) tosses);
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
