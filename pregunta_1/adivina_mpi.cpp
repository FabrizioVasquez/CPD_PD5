#include <mpi.h>
#include <math.h>
#include <ctime>
#include <cstdlib>  
#include <iostream>
#include <random>

template<typename T>
T random_generate(T range_from, T range_to) {
    // https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<T>    distr(range_from, range_to);
    return distr(generator);
}

static const int START_RANGE = 1;
static const int END_RANGE   = 100;

int main(int argc, char **argv) {
  
  int rank, size;
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processors
  std::string paolo="Paolo";
  int paolo_number{};
  int friend_number{};
  int times = 0;
  while(times < 2){
    if(rank == 0)
    {
        std::cout<<"Paolo is thinking about his life. When... "<<std::endl;
        paolo_number = random_generate<int>(START_RANGE,END_RANGE);
        std::cout<<"Paolo decide the number and this is ... "<<paolo_number<<std::endl;
    }
    else
    {
        std::cout<<"Thinking about the number that chose Paolo..."<<std::endl;
        friend_number = random_generate(START_RANGE,END_RANGE);
        std::cout<<"Paolo's friend choose this number..."<<friend_number<<std::endl;
    }
    times++;
  }
  MPI_Finalize();
}