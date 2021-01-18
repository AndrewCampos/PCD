#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>

#define NUM_GEN 2000 // Numero de geracoes
#define TAM 2048 // Tamanho N da matriz NxN
#define MAX_THREADS 1
#define SRAND_VALUE 1985
#define vivo 1
#define morto 0

int **grid, **newgrid;

struct thread_data{
   int offset;
}thread_data;

typedef struct {
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

void *verificaCelula(void *th){
    int i,j,offset;
    struct thread_data *arg;
    arg = (struct thread_data *) th;
    offset = arg->offset;
    for(i=0; i<TAM/MAX_THREADS; i++){
        for(j = 0; j<TAM; j++){
            if (grid[i+offset][j]){ // Se estiver vivo
                if (getNeighbors(i+offset,j) < 2 || getNeighbors(i+offset,j) > 3) // Regra A e C
                    newgrid[i+offset][j] = morto;
                else // Regra B
                    newgrid[i+offset][j] = vivo; 
            }
            else{ // Se estiver morto
                if(getNeighbors(i+offset,j) == 3) // Regra D
                    newgrid[i+offset][j] = vivo;
                else
                    newgrid[i+offset][j] = morto;
            }
        }
    }  
    pthread_exit(NULL);
}

// Cria uma nova geracao de acordo com as regras estabelecidas
void novaGeracao(){
    int i, j;
    pthread_t th[MAX_THREADS];
    struct thread_data param[MAX_THREADS];
    
    for(i=0; i<MAX_THREADS; i++){   
        param[i].offset = i*(TAM/MAX_THREADS);
        pthread_create(&th[i], NULL, verificaCelula, (void *)&param[i]);
    }

    for (j=0;j<MAX_THREADS;j++){
        pthread_join(th[j],NULL);
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

// Imprime as duas primeiras geracoes
/*void imprimeGeracao(){
    int i, j;
    for(i=0;i<TAM; i++){     
        for(j = 0; j<TAM; j++){
            printf("%d ",grid[i][j]);
        }
        printf("\t");
        for(j = 0; j<TAM; j++){
            printf("%d ",newgrid[i][j]);
        }
        printf("\n");
    }
}*/

// Imprime matriz que indica numero de vizinhos de cada posicao
/*void imprimeMatrizVizinhos(){
    int i, j;
    
    for(i=0;i<TAM; i++){     
        for(j = 0; j<TAM; j++){
            printf("%d ",getNeighbors(i,j));
        }
        printf("\n");
    }
}*/

int main(){
    int i, j;
    TIME_DIFF *time;
    struct timeval start, end;

    gettimeofday (&start, NULL);

    // Alocacao das matrizes
    grid = malloc(sizeof(int*)*TAM);
    newgrid = malloc(sizeof(int*)*TAM);
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

    printf("Ultima Geracao: %d Celulas Vivas\n", contaPopulacao());

    gettimeofday (&end, NULL);
    time = my_difftime(&start, &end);
    printf("Tempo: %dseg\n",time->secs);

    return 0;
}
