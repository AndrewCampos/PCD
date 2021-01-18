#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<omp.h> 

#define NUM_GEN 2000 // Numero de geracoes
#define TAM 2048 // Tamanho N da matriz NxN
#define SRAND_VALUE 1985
#define MAX_THREADS 2
#define vivo 1
#define morto 0
#define OCUPADO 1
#define LIVRE 0

typedef struct {
    int secs;
    int usecs;
}TIME_DIFF;

typedef int BOOL;

int **grid, **newgrid, *senhas, segs, usegs;
BOOL critical_sec = LIVRE;

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
    
    #pragma omp parallel private(j) num_threads(MAX_THREADS)
    #pragma omp for
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

    #pragma omp parallel private(j) num_threads(MAX_THREADS)
    #pragma omp for
    for(i=0;i<TAM; i++){     
        for(j = 0; j<TAM; j++){
            grid[i][j] = newgrid[i][j];
        }
    }
}

// Valor da maior senha
int MaxSenhas(){
    int i,max=0;
    for(i=0;i<MAX_THREADS;i++){
        if(senhas[i] > max) max = senhas[i];
    }
    return max;
}

// Valor da maior senha
int MinSenhas(){
    int i,min=100000;
    for(i=0;i<MAX_THREADS;i++){
        if(senhas[i] < min && senhas[i] > 0) min = senhas[i];
    }
    return min;
}

void imprimeSenhas(){
    int i;
    for(i=0;i<MAX_THREADS;i++) printf("%d ", senhas[i]);
    printf("\n");
}

// Conta quantas celulas estao vivas na geracao
int contaPopulacao(){
    int i,j,ID,cont = 0;
    
    #pragma omp parallel private(ID,ini,fim) num_threads(MAX_THREADS)
    #pragma omp for
        for(i=0;i<TAM; i++){
            ID = omp_get_thread_num();
            senhas[ID] = MaxSenhas() + 1;
            while(MaxSenhas() != 0 && senhas[ID] > MinSenhas());
            // Entrada na sessao critica
            for(j = 0; j<TAM; j++){
                if (grid[i][j]){
                    cont++;
                }
            }
            // Saida da sessao critica
            senhas[ID] = 0;
        }
    return cont;
}

int main(){
    int i, j;
    TIME_DIFF *time;
    struct timeval start, end;

    // Alocacao das matrizes
    grid = malloc(sizeof(int*)*TAM);
    newgrid = malloc(sizeof(int*)*TAM);
    senhas = malloc(sizeof(int)*MAX_THREADS);
    for(i=0;i<TAM;i++){
        grid[i] = malloc(sizeof(int)*TAM);
        newgrid[i] = malloc(sizeof(int)*TAM);
    }

    // Gera a primeira geracao pseudoaleatoriamente
    srand(SRAND_VALUE);
    for(i=0;i<TAM; i++){     
        for(j = 0; j<TAM; j++){
            grid[i][j] = rand() % 2;  
        }
    }

    printf("Condicao Inicial: %d Celulas Vivas\n", contaPopulacao());

    // Gera NUM_GEN geracoes a partir da primeira
    for(i=0;i<NUM_GEN;i++){
        novaGeracao();
    }

    gettimeofday (&start, NULL);
    printf("Ultima Geracao: %d Celulas Vivas\n", contaPopulacao());
    gettimeofday (&end, NULL);
    
    time = my_difftime(&start, &end);
    printf("Tempo: %d,%d s\n",time->secs,time->usecs);

    return 0;
}