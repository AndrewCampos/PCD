#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/time.h>

#define NUM_GEN 2000 // Numero de geracoes
#define TAM 1024 // Tamanho N da matriz NxN
#define SRAND_VALUE 1985
#define vivo 1
#define morto 0

int **grid, **newgrid;

typedef struct{
    int secs;
    int usecs;
}TIME_DIFF;

// Calcula o tempo decorrido entre um intervalo de tempo
TIME_DIFF * my_difftime (struct timeval *start, struct timeval *end){
    TIME_DIFF *diff = (TIME_DIFF*)malloc(sizeof(TIME_DIFF));
 
    if(start->tv_sec == end->tv_sec){
        diff->secs = 0;
        diff->usecs = end->tv_usec - start->tv_usec;
    }
    else{
        diff->usecs = 1000000 - start->tv_usec;
        diff->secs = end->tv_sec - (start->tv_sec + 1);
        diff->usecs += end->tv_usec;
        if(diff->usecs >= 1000000){
            diff->usecs -= 1000000;
            diff->secs += 1;
        }
    }
    return diff;
}

// Retorna a quantidade de vizinhos vivos de cada celula na posicao ​i,j
int getNeighbors(int i, int j) {
    int count=0;

    count += grid[i][((j+1)%TAM)]; // direita
    count += grid[((i+1)%TAM)][((j+1)%TAM)]; // direita baixo
    count += grid[((i+1)%TAM)][j]; //baixo
    count += grid[((i+1)%TAM)][(TAM+(j-1))%TAM]; // esquerda baixo
    
    count += grid[i][(TAM+(j-1))%TAM]; // esquerda
    count += grid[(TAM+(i-1))%TAM][(TAM+(j-1))%TAM]; //esquerda cima
    count += grid[(TAM+(i-1))%TAM][j]; // cima
    count += grid[(TAM+(i-1))%TAM][((j+1)%TAM)]; // direita cima

    return count;
}

// Cria uma nova geracao de acordo com as regras estabelecidas
void novaGeracao(){
    int i, j;
    
    for(i=0;i<TAM; i++){     
        for(j = 0; j<TAM; j++){
            if (grid[i][j]){ // Se estiver vivo
                if (getNeighbors(i,j) < 2 || getNeighbors(i,j) > 3) // Regra A e C
                    newgrid[i][j] = morto;
                else // Regra B
                    newgrid[i][j] = vivo; 
            }
            else{ // Se estiver morto
                if(getNeighbors(i,j) == 3) // Regra D
                    newgrid[i][j] = vivo;
                else
                    newgrid[i][j] = morto;
            }
        }
    }

    for(i=0;i<TAM; i++){     
        for(j = 0; j<TAM; j++){
            grid[i][j] = newgrid[i][j];
        }
    }
}

// Conta quantas celulas estao vivas na geracao
int contaPopulacao(){
    int i,j,cont = 0;

    for(i=0;i<TAM; i++){     
        for(j = 0; j<TAM; j++){
            if (grid[i][j])
                cont++;
        }
    }

    return cont;
}

// Rotina do processo principal
void prinProc(int numProc){
    int i,j,origem,tag=0,div=0;
    int bufTam = TAM*(TAM/numProc);
    int bufRcv[TAM/numProc][TAM];
    TIME_DIFF *time;
    struct timeval start, end;

    printf("Sistema principal iniciado!\n");
    gettimeofday (&start, NULL);
    
    // Alocacao das matrizes
    grid = malloc(sizeof(int*)*TAM);
    newgrid = malloc(sizeof(int*)*TAM);
    for(i=0;i<TAM;i++){
        grid[i] = malloc(sizeof(int)*TAM);
        newgrid[i] = malloc(sizeof(int)*TAM);
    }
    // Gera a primeira metade da primeira geração pseudoaleatoriamente
    i=1;
    srand(SRAND_VALUE);
    for(i=0;i<(TAM/numProc); i++){     
        for(j = 0; j<TAM; j++){
            grid[i][j] = rand() % 2;  
        }
    }
    div = i;
    for(origem=1;origem<numProc;origem++){
        MPI_Recv(bufRcv,bufTam,MPI_INT,origem,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("Mensagem recebida!\n");
        for(i=0;i<TAM/numProc;i++){
            for(j=0;j<TAM;j++){
                grid[i+div][j] = bufRcv[i][j];
            }
        }
        div+=i;
    }
    tag++;

    printf("Condicao Inicial: %d Celulas Vivas\n", contaPopulacao());

    // Gera NUM_GEN geracoes a partir da primeira
    for(i=0;i<NUM_GEN;i++){
        novaGeracao();
    }

    printf("Ultima Geracao: %d Celulas Vivas\n", contaPopulacao());

    gettimeofday (&end, NULL);
    time = my_difftime(&start, &end);
    printf("Tempo: %d,%d s\n",time->secs,time->usecs);
}

// Rotina do processo secundario
void secProc(int numProc){
    int tag=0,dest=0,i,j, bufTam = TAM*(TAM/numProc);
    int bufSnd[TAM/numProc][TAM];
    printf("Sistema secundario iniciado!\n");

    // Gera a segunda metade da primeira geracao pseudoaleatoriamente
    srand(SRAND_VALUE);
    for(i=0;i<(TAM/numProc); i++){     
        for(j = 0; j<TAM; j++){
            bufSnd[i][j] = rand() % 2;  
        }
    }

    MPI_Send(bufSnd,bufTam,MPI_INT,dest,tag,MPI_COMM_WORLD);
}

int main(int argc, char *argv[]){
    int rank; // ID do processo
    int numProc; // numero de processos

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0){ // caso seja o processo principal
        prinProc(numProc);
    }else{
        secProc(numProc);
    }

    MPI_Finalize();
    return 0;
}