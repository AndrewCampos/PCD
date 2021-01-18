class RunTh implements Runnable {
	private int TAM;
	private int MaxThreads;
	private int Offset;
	private int grid[][];
	 int gridAux[][];
    
     /* construtor */
	public RunTh(int tam, int maxThreads, int offset,int genAtual[][]) {
		this.TAM = tam;
		this.MaxThreads = maxThreads;
		this.Offset = offset;
		this.grid = genAtual;
		this.gridAux = new int[(tam/this.MaxThreads)][tam];
	}

	/* Retorna a quantidade de vizinhos vivos de cada celula na posicao ​i,j */
	public int getNeighbors(int i, int j) {
		int count=0;
	
		count += grid[i][((j+1)%this.TAM)]; // direita
		count += grid[((i+1)%this.TAM)][((j+1)%this.TAM)]; // direita baixo
		count += grid[((i+1)%this.TAM)][j]; //baixo
		count += grid[((i+1)%this.TAM)][(this.TAM+(j-1))%this.TAM]; // esquerda baixo
		
		count += grid[i][(this.TAM+(j-1))%this.TAM]; // esquerda
		count += grid[(this.TAM+(i-1))%this.TAM][(this.TAM+(j-1))%this.TAM]; //esquerdacima
		count += grid[(this.TAM+(i-1))%this.TAM][j]; // cima
		count += grid[(this.TAM+(i-1))%this.TAM][((j+1)%this.TAM)]; // direita cima

		return count;
	}
    
    /* Metodo rodado pela thread */
	public void run() {
		for(int i=0;i<(this.TAM/this.MaxThreads); i++){     
            for(int j = 0; j<this.TAM; j++){
                if (this.grid[(i+this.Offset)][j] == 1){ // Se estiver vivo
                    if (getNeighbors((i+this.Offset),j) < 2 || getNeighbors((i+this.Offset),j) > 3){ // Regra A e C
                        this.gridAux[i][j] = 0;
                    }
                    else // Regra B
                        this.gridAux[i][j] = 1; 
                }
                else{ // Se estiver morto
                    if(getNeighbors((i+this.Offset),j) == 3) // Regra D
                        this.gridAux[i][j] = 1;
                    else
                        this.gridAux[i][j] = 0;
                }
            }
        }
	}
}