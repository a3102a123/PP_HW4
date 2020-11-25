#include<bits/stdc++.h>
#include <mpi.h>
void construct_matrices(int *n_ptr, int *m_ptr, int *l_ptr,int **a_mat_ptr, int **b_mat_ptr){
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    printf("Construct Matrix %d\n",world_rank);
    /*only MPI_COMM_WORLD rank 0 inherits stdin*/
    if (world_rank == 0)
    {
        scanf("%d %d %d",n_ptr,m_ptr,l_ptr);
        a_mat_ptr = (*int) malloc( sizeof(int) * (*n_ptr) * (*m_ptr));
        a_mat_ptr = (*int) malloc( sizeof(int) * (*m_ptr) * (*l_ptr));
        for(int i = 0 ; i < n_ptr ; i++)
            for(int j = 0 ; j < m_ptr ; j++)
                scanf("%d",a_mat_ptr + i*(*m_ptr) + j);
        for(int i = 0 ; i < m_ptr ; i++)
            for(int j = 0 ; j < l_ptr ; j++)
                scanf("%d",b_mat_ptr + i*(*l_ptr) + j);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

void matrix_multiply(const int n, const int m, const int l,const int *a_mat, const int *b_mat){
    int world_rank, world_size;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    printf("Calctulate Matrix in %d : %d\n",world_rank,*a_mat);
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