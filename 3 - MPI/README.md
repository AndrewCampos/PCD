## Procedimento para executar programa em MPI

+ **Instalar:**
  - sudo apt-get install libopenmpi-dev openmpi-bin libhdf5-openmpi-dev
+ **Compilar:**
  - mpicc <nome do arquivo> -o <nome da saída>
+ **Executar:**
  - mpirun -np <num processos> <arquivo compilado>

**Exemplo para _hello\_mpi.c_**

```
$ mpicc hello_mpi.c -o hello.e
$ mpirun -np 2 hello.e
```