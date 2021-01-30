#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){
    int ierr,destino,origem,tag=0;
    int rank; // Id do processo
    char bufRcv[5], **bufSnd;

    bufSnd = malloc(5*sizeof(char));
    bufSnd = {1,2,3,4,5};
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if(rank == 0){
        destino = rank+1;
        ierr = MPI_Send(bufSnd,5,MPI_INT,destino,tag,MPI_COMM_WORLD);
        printf("Processo %d enviou: %d\n""Send Status: %d\n",rank,bufSnd[0][0],ierr);
    }else{
        origem = rank-1;
        ierr = MPI_Recv(bufRcv,5,MPI_INT,origem,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("Processo 0 recebeu: %d\n""Receive Status: %d\n",bufSnd[0][0],ierr);
    }

    MPI_Finalize();
    return 0;
}
