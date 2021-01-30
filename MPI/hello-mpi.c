#include "mpi.h"
#include <stdio.h>

int main(int argc, char const *argv[]){
    int process_id; // rank dos processos
    int noProcess;  // numero de processos
    int nameSize;   // tamanho do nome
    char computerName[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &noProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Get_processor_name(computerName, &nameSize);

    printf("Hello from process %d/%d on %s\n", process_id, noProcess, computerName);
    MPI_Finalize();
    return 0;
}
