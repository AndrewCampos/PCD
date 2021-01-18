import java.util.Random;

public class Serial {
    private int TAM;
    public int grid[][];
    public int newgrid[][];

    // Construtor
    public Serial(int dim){
        this.TAM = dim;
        this.grid = new int[this.TAM][this.TAM];
        this.newgrid = new int[this.TAM][this.TAM];

        Random gerador = new Random(1985);
        for(int i = 0; i<this.TAM; i++) {   
            for(int j = 0; j<this.TAM; j++) {
                grid[i][j] = gerador.nextInt(2147483647) % 2;  
            }
        }
    }

    // Imprime as duas primeiras geracoes
    public void printGen(){
        for(int i=0;i<this.TAM;i++){
            for(int j = 0; j<this.TAM; j++){
                System.out.print(grid[i][j] + " ");
            }
            System.out.print("      ");
            for(int j = 0; j<this.TAM; j++){
                System.out.print(newgrid[i][j] + " ");
            }
            System.out.println("");
        }
    }

    // Retorna a quantidade de vizinhos vivos de cada celula na posicao ​i,j
    public int getNeighbors(int i, int j) {
        int count=0;

        count += grid[i][((j+1)%this.TAM)]; // direita
        count += grid[((i+1)%this.TAM)][((j+1)%this.TAM)]; // direita baixo
        count += grid[((i+1)%this.TAM)][j]; //baixo
        count += grid[((i+1)%this.TAM)][(this.TAM+(j-1))%this.TAM]; // esquerda baixo
        
        count += grid[i][(this.TAM+(j-1))%this.TAM]; // esquerda
        count += grid[(this.TAM+(i-1))%this.TAM][(this.TAM+(j-1))%this.TAM]; //esquerda cima
        count += grid[(this.TAM+(i-1))%this.TAM][j]; // cima
        count += grid[(this.TAM+(i-1))%this.TAM][((j+1)%this.TAM)]; // direita cima

        return count;
    }

    // Imprime matriz que indica numero de vizinhos de cada posicao
    public void printMatNeighbors(){    
        for(int i=0;i<this.TAM; i++){     
            for(int j = 0; j<this.TAM; j++){
                System.out.print(this.getNeighbors(i,j) + " ");
            }
            System.out.println("");
        }
    }

    // Cria uma nova geracao de acordo com as regras estabelecidas
    public void newGen(){
        for(int i=0;i<this.TAM; i++){     
            for(int j = 0; j<this.TAM; j++){
                if (grid[i][j] == 1){ // Se estiver vivo
                    if (getNeighbors(i,j) < 2 || getNeighbors(i,j) > 3){ // Regra A e C
                        //System.out.println(i + " " + j + " " + getNeighbors(i, j) + " " + grid[i][j]);
                        this.newgrid[i][j] = 0;
                    }
                    else // Regra B
                        this.newgrid[i][j] = 1; 
                }
                else{ // Se estiver morto
                    if(getNeighbors(i,j) == 3) // Regra D
                        this.newgrid[i][j] = 1;
                    else
                        this.newgrid[i][j] = 0;
                }
            }
        }

        for(int i=0;i<this.TAM; i++){     
            for(int j = 0; j<this.TAM; j++){
                grid[i][j] = newgrid[i][j];
            }
        }
    }

    // Conta quantas celulas estao vivas na geracao
    public int contPopulation(){
        int cont = 0;

        for(int i=0;i<this.TAM; i++){     
            for(int j = 0; j<this.TAM; j++){
                if (this.grid[i][j] == 1)
                    cont++;
            }
        }

        return cont;
    }

    public static void main(String[] args){
        Serial gen1 = new Serial(2048);
        
        long startTime = System.currentTimeMillis();
        System.out.println("Condição Inicial: " + gen1.contPopulation() + " Celulas Vivas");
        for(int i=0;i<2000;i++){
            gen1.newGen();
        }
        System.out.println("Geração 2000: " + gen1.contPopulation() + " Celulas Vivas");
        double calcTime = (System.currentTimeMillis() - startTime)/1000.0;
        System.out.println(calcTime + " seg");
    }
}
