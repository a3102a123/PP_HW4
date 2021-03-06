#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <math.h>

using namespace std;
double inv_max = 2.0 / RAND_MAX;

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    long long int range = ceil(tosses / (float)world_size) , begin_idx = 0,begin,end; 
    static unsigned int seed = time(NULL) * world_rank;
    double distance_squared,x,y;
    long long int count = 0,sum;
    int root = 0;

    begin = range * world_rank;
    end = min(range * (world_rank + 1) , tosses);

    // TODO: handle workers
    for(int i = begin ; i < end ; i++){
        x = (double)rand_r(&seed) * inv_max  + (-1.0);
        y = (double)rand_r(&seed) * inv_max  + (-1.0);
        distance_squared = x * x + y * y;
        if ( distance_squared <= 1)
            count++;
    }

    if (world_rank > 0)
    {
        MPI_Reduce(&count ,NULL ,1 ,MPI_LONG_LONG ,MPI_SUM,root ,MPI_COMM_WORLD );
    }
    else if (world_rank == 0)
    {
        MPI_Reduce(&count ,&sum ,1 ,MPI_LONG_LONG ,MPI_SUM,root ,MPI_COMM_WORLD );
        // TODO: master
    }

    if (world_rank == 0)
    {
        // TODO: PI result
        pi_result = 4.0 * (double)sum /(( double ) tosses);
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
