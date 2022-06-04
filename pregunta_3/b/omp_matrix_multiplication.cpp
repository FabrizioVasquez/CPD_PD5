#include <omp.h>
#include <random>
#include <iostream>
using namespace std;

int main(int argc, char** argv){
    srand(time(NULL));
    int num = strtol(argv[1], NULL, 10);
    int N = num;
    int m[N][N];
    int v[N];
    int r[N];
    double t0 = 0.0;
    double tf = 0.0;
    int i, j;
    t0 = omp_get_wtime();
    #pragma omp parallel private(i, j) num_threads(num)
    {
        #pragma omp single
            for(i = 0; i < N; i++) v[i] = 1 + rand()% 5;
        
        #pragma omp single
            for(i = 0; i < N ;i++)
                for(j = 0; j < N; j++)
                    m[i][j] = 1 + rand()%5;

        #pragma omp for schedule(static)
            for(i = 0; i < N; i++){
                r[i] = 0;
                for(j=0; j < N; j++){
                    r[i] += m[i][j]*v[j];
                }
            }
    }
    tf = omp_get_wtime();
    cout << tf - t0 << endl;
}