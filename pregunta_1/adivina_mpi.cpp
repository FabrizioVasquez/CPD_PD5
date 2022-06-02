#include <mpi.h>
#include <math.h>
#include <ctime>
#include <cstdlib>  
#include <iostream>
#include <random>
#include <fstream>

using namespace std;

static const int START_RANGE = 1;
static const int END_RANGE   = 100;

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

  static const int MAX_ITER = 1000;
  int attempt = 0;

  int aquiles_number{};
  int mirmidon_number{};
  bool mirmidon_bool{};
  bool reduccion_land_result = false;
  int result;

  while(attempt < MAX_ITER){
    if(rank == 0)
    {
      aquiles_number = random_generate<int>(START_RANGE, END_RANGE);
    
    }

    MPI_Bcast(&aquiles_number,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    mirmidon_number = random_generate(START_RANGE,END_RANGE);
    mirmidon_bool = (aquiles_number == mirmidon_number)?true:false;
    
    MPI_Reduce(&mirmidon_bool,&reduccion_land_result,1,MPI_C_BOOL,MPI_LAND,0,MPI_COMM_WORLD);

    if(rank == 0){
      if(reduccion_land_result){
        std::cout<<std::boolalpha<<"Todos los Mirmidones estan de acuerdo? "<<reduccion_land_result<<std::endl;
        std::cout<<"Pasaron: "<<attempt+1<<" intentos."<<std::endl;
        result = 1;
        break;
      }else 
        result = 0;
    }
    attempt++;
  }

  if(rank == 0){  
    string folder = "";
    string filename = folder + "resultados.txt";
    ofstream file(filename, ios::out | ios::app);
    file << result << endl;
    file.close();
  }

  MPI_Finalize();
}