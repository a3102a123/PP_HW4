#include<bits/stdc++.h>
#include <mpi.h>
void construct_matrices(int *n_ptr, int *m_ptr, int *l_ptr,int **a_mat_ptr, int **b_mat_ptr){
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    printf("Construct Matrix %d\n",world_rank);
    if (world_rank > 0)
    {
        scanf("%d %d %d",n_ptr,m_ptr,l_ptr);
    }
    else if (world_rank == 0)
    {
        scanf("%d %d %d",n_ptr,m_ptr,l_ptr);
    }
    printf("%d %d %d\n",*n_ptr,*m_ptr,*l_ptr);
}

void matrix_multiply(const int n, const int m, const int l,const int *a_mat, const int *b_mat){
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    printf("Calctulate Matrix\n");
    if (world_rank > 0)
    {
    }
    else if (world_rank == 0)
    {
    }
}

void destruct_matrices(int *a_mat, int *b_mat){
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    printf("Destroy Matrix\n");
    if (world_rank > 0)
    {
    }
    else if (world_rank == 0)
    {
    }
}