#include<bits/stdc++.h>
#include <mpi.h>

using namespace std;

void construct_matrices(int *n_ptr, int *m_ptr, int *l_ptr,int **a_mat_ptr, int **b_mat_ptr){
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    // printf("Construct Matrix %d\n",world_rank);
    /*only MPI_COMM_WORLD rank 0 inherits stdin*/
    if (world_rank == 0)
    {
        scanf("%d %d %d",n_ptr,m_ptr,l_ptr);
    }
    // synchronize data
    MPI_Bcast(n_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(m_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(l_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
    *a_mat_ptr = (int *) malloc( sizeof(int) * (*n_ptr) * (*m_ptr));
    *b_mat_ptr = (int *) malloc( sizeof(int) * (*m_ptr) * (*l_ptr));
    if(world_rank == 0)
    {
        for(int i = 0 ; i < *n_ptr ; i++)
            for(int j = 0 ; j < *m_ptr ; j++)
                scanf("%d",*a_mat_ptr + i*(*m_ptr) + j);
        for(int i = 0 ; i < *m_ptr ; i++)
            for(int j = 0 ; j < *l_ptr ; j++)
                scanf("%d",*b_mat_ptr + i*(*l_ptr) + j);
    }
    MPI_Bcast(*a_mat_ptr, (*n_ptr) * (*m_ptr), MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(*b_mat_ptr, (*m_ptr) * (*l_ptr), MPI_INT, 0, MPI_COMM_WORLD);
}

void matrix_multiply(const int n, const int m, const int l,const int *a_mat, const int *b_mat){
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    // printf("Calctulate Matrix in %d : %d\n",world_rank,*(a_mat+1));
    // setup variable
    int begin , end , block_size = ceil(n / (float)world_size);
    begin = world_rank * block_size;
    end = min( (world_rank + 1) * block_size , n);
    // printf("Process %d run [ %d - %d ]\n",world_rank,begin,end);
    int *result;
    int result_size;
    if (world_rank == 0)
    {
        result_size = n * l;
        result = (int *)malloc(sizeof(int) * result_size);
    }
    else if (world_rank > 0)
    {
        result_size = (end - begin) * l;
        result = (int *)malloc(sizeof(int) * result_size);
    }
    // calculate the matrix multiply
    const int *a,*b;
    int *ptr;
    for(int i = begin ; i < end ; i++ ){
        for(int j = 0 ; j < l ; j++){
            ptr = result + (i - begin) * l + j;
            *ptr = 0;
            for(int k = 0 ; k < m ; k++){
                a = a_mat + i * m + k;
                b = b_mat + k * l + j;
                // printf("%d * %d\n",*a,*b);
                *ptr += (*a) * (*b);
            }
        }
    }
    // send result to master
    if(world_rank == 0)
    {
        for(int i = 1 ; i < world_size ; i++){
            begin = i * block_size;
            end = min( (i + 1) * block_size , n);
            result_size = (end - begin) * l;
            ptr = result + begin * l;
            // printf("Receive from process %d begin %d . size %d\n",world_rank,begin,result_size);
            MPI_Recv(ptr, result_size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

    }
    else if (world_rank > 0){
        MPI_Send(result ,result_size ,MPI_INT,0 ,0 ,MPI_COMM_WORLD );
    }
    // print result
    if(world_rank == 0){
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < l ; j++){
                printf("%d ",*(result + (i * l) + j));
            }
            printf("\n");
        }
    }
    free(result);
}

void destruct_matrices(int *a_mat, int *b_mat){
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    // printf("Destroy Matrix\n");
    free(a_mat);
    free(b_mat);
}