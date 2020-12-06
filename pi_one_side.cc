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

int fnz (long long int *schedule, long long int *oldschedule, int size, long long int *count){
    int diff = 0;

    for (int i = 0; i < size; i++)
        diff |= (schedule[i] != oldschedule[i]);

    if (diff)
    {
        int res = 0;

        for (int i = 0; i < size; i++)
        {
            if(schedule[i] != oldschedule[i]){
                // printf("Receive %lld from %d\n", schedule[i],i);
                (*count) += schedule[i];
            }
            if(schedule[i] != 0)
                res += 1;
            oldschedule[i] = schedule[i];
        }

        return(res == size-1);
    }
    return 0;

}

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    MPI_Win win;

    // TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    long long int range = ceil(tosses / (float)world_size) , begin_idx = 0,begin,end; 
    static unsigned int seed = time(NULL) * world_rank;
    double distance_squared,x,y;
    long long int count = 0;
    int root = 0;

    begin = range * world_rank;
    end = min(range * (world_rank + 1) , tosses);

    srand(seed);
    for(int i = begin ; i < end ; i++){
        x = (double)rand() * inv_max  + (-1.0);
        y = (double)rand() * inv_max  + (-1.0);
        distance_squared = x * x + y * y;
        if ( distance_squared <= 1)
            count++;
    }

    if (world_rank == 0)
    {
        // Master
        long long int *oldschedule = (long long int*) malloc(world_size * sizeof(long long int));
        long long int *schedule;
        MPI_Alloc_mem(world_size * sizeof(long long int), MPI_INFO_NULL, &schedule);
        for (int i = 0; i < world_size; i++)
        {
            schedule[i] = 0;
            oldschedule[i] = 0;
        }
        MPI_Win_create(schedule, world_size * sizeof(long long int), sizeof(long long int), MPI_INFO_NULL,MPI_COMM_WORLD, &win);
        int ready = 0;
        while (!ready)
        {
            MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
            ready = fnz(schedule, oldschedule, world_size,&count);
            MPI_Win_unlock(0, win);
        }

        MPI_Free_mem(schedule);
        free(oldschedule);
    }
    else
    {
        // Workers
        MPI_Win_create(NULL, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, 0, win);
        MPI_Put(&count, 1, MPI_LONG_LONG, 0, world_rank, 1, MPI_LONG_LONG, win);
        MPI_Win_unlock(0, win);

        // printf("%d send %lld\n",world_rank,count);
    }

    MPI_Win_free(&win);

    if (world_rank == 0)
    {
        // TODO: handle PI result
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