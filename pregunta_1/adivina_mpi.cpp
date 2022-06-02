#include <mpi.h>
#include <math.h>
#include <ctime>
#include <cstdlib>  
#include <iostream>
#include <random>

static const int START_RANGE = 1;
static const int END_RANGE   = 2;

template<typename T>
T random_generate(T range_from, T range_to) {
    // https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<T>    distr(range_from, range_to);
    return distr(generator);
}

int main(int argc, char **argv) {
  
  int rank, size;
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processors

  //static const int NFRIENDS = 5;
  static const int MAX_ITER = 1000;
  int attempt = 0;

  int aquiles_number{};
  int mirmidon_number{};
  bool mirmidon_bool{};
  bool reduccion_land_result = false;
  double t0 = 0.0;
  double tf = 0.0;

  while(true){
    if(rank == 0)
    {
      //t0 = MPI_Wtime();
      //std::cout<<"Aquiles is thinking about his life. When... "<<std::endl;
      aquiles_number = random_generate<int>(START_RANGE,END_RANGE);
      //std::cout<<"Aquiles decide the number and this is ... "<<aquiles_number<<std::endl;
      //Aquiles();
    }

    MPI_Bcast(&aquiles_number,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    mirmidon_number = random_generate(START_RANGE,END_RANGE);
    mirmidon_bool = (aquiles_number == mirmidon_number)?true:false;
    
    MPI_Reduce(&mirmidon_bool,&reduccion_land_result,1,MPI_C_BOOL,MPI_LAND,0,MPI_COMM_WORLD);
    //std::cout<<"Mirmidon "<<rank<<" choose this number..."<<mirmidon_number<<std::endl;

    //std::cout<<"Mirmidon "<<rank<<" posee una copia del numero de Aquiles: "<<aquiles_number<<std::endl;

    
    if(rank == 0){
      if(reduccion_land_result){
        //tf = MPI_Wtime();
        std::cout<<std::boolalpha<<"Todos los Mirmidones estan de acuerdo? "<<reduccion_land_result<<std::endl;
        //std::cout<<"Nos llevo un tiempo de "<<tf-t0<<" encontrar que los mirmidones se encuentren decuacuerdo"<<std::endl;
        // el tiempo no se esta calculando correctamente
        std::cout<<"Pasaron: "<<attempt<<" intentos."<<std::endl;
        //break;
        MPI_Abort(MPI_COMM_WORLD , 911);
      }
    }
    attempt++;
  }

  MPI_Finalize();
}