#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){
    int ierr,destino,origem,tag=0;
    int rank; // Id do processo
    int bufRcv[5][2], bufSnd[5][2] = {{0,1},{0,2},{0,3},{0,4},{0,5}};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(rank == 0){
        destino = rank+1;
        ierr = MPI_Send(bufSnd,5*2,MPI_INT,destino,tag,MPI_COMM_WORLD);
        printf("Processo %d enviou: %d\n""Send Status: %d\n",rank,bufSnd[1][1],ierr);
    }else{
        origem = rank-1;
        ierr = MPI_Recv(bufRcv,5*2,MPI_INT,origem,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("Processo 0 recebeu: %d\n""Receive Status: %d\n",bufSnd[1][1],ierr);
    }

    MPI_Finalize();
    return 0;
}
