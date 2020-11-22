#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

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
    long long int count = 0;

    begin = range * world_rank;
    end = min(range * (world_rank + 1) , tosses);

    srand(seed);

    // TODO: binary tree redunction
    for(int i = begin ; i < end ; i++){
        x = (double)rand() * inv_max  + (-1.0);
        y = (double)rand() * inv_max  + (-1.0);
        distance_squared = x * x + y * y;
        if ( distance_squared <= 1)
            count++;
    }

    int id = world_rank , time = 1;
    int dst , source;
    MPI_Status status; 
    long long int temp;

    while (time <= world_size){
        if( id % 2){
            dst = world_rank - time;
            MPI_Send(&count ,1 ,MPI_LONG_LONG,dst ,0 ,MPI_COMM_WORLD );
            break;
        }
        else{
             MPI_Recv(&temp, 1, MPI_LONG_LONG, source, 0, MPI_COMM_WORLD, &status);
            count += temp;
        }
        time *= 2;
        id /= 2;
    }
    

    if (world_rank == 0)
    {
        // TODO: PI result
        pi_result = 4.0 * (double)count /(( double ) tosses);
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
