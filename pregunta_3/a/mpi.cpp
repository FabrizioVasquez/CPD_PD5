#include <mpi.h>
#include <iostream>
#include <random>
#include <fstream>

int main(int argc, char **argv) {

    int rank, size;
    MPI_Init(&argc, &argv);                   // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rank of the processor
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processors
    
    int N = size;
    int m[N][N];
    int v[N];
    int r[N];
    int r_p[N];
    int fila_local[N];
    double t0 = 0.0;
    double tf = 0.0;

    if(rank == 0){
        // Secuencial
        srand(time(NULL));
        for(int i=0; i < N ;i++){
            v[i] = 1+ rand()% 5;
            for(int j=0; j < N; j++){
                m[i][j] = 1 + rand()%5;
            }
        }

        for(int i = 0; i < N;i++){
            r[i] = 0 ;
            for(int j=0; j < N; j++){
                r[i] += m[i][j]*v[j];
            }
        }

        std::cout<<std::endl;
        t0 = MPI_Wtime();
    }
    
    MPI_Scatter( m[0] , N , MPI_INT , fila_local , N , MPI_INT , 0 , MPI_COMM_WORLD);
    MPI_Barrier( MPI_COMM_WORLD);
    
    //Copia de los v para cada procesador
    MPI_Bcast( v , N , MPI_INT , 0 , MPI_COMM_WORLD);
    MPI_Barrier( MPI_COMM_WORLD);
    //Copia de los v para cada procesador

    int tmp_sum = 0;
    for(int i = 0; i < N; i++){
        tmp_sum += fila_local[i]*v[i];
    }

    MPI_Gather( &tmp_sum , 1 , MPI_INT , r_p , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
    tf = MPI_Wtime();

    if(rank == 0){
        std::ofstream file("tiempos_mpi.txt", std::ios::out | std::ios::app);
        file << (tf-t0)<<std::endl;
        file.close();
    }

    MPI_Finalize();
}