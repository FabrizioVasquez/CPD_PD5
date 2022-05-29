#include<iostream>
#include<omp.h>
using namespace std;
//#pragma omp parallel num_threads(5)

void restart(double A[], double B[], double C[], int n){
    for(int i = 0; i < n; i++){
        A[i] = 1;
        B[i] = 1;
        C[i] = 1;
    }
}
void calculo_a(double A[], int n){
    for(int i = 0; i < n; ++i) A[i] = 1;
}

void calculo_b(double B[], int n){
    for(int i = 0; i < n; ++i) B[i] = 1;
}

void calculo_c(double C[],int n){
    for(int i = 0; i < n; ++i) C[i] = 1;
}

double OpArr(double A[], double B[], double C[], int n)
{
    int i, j;
    double s1, s2, a, res;
    calculo_a(A, n); /* funcion calculo a */
    calculo_b(B, n); /* funcion calculo b */
    calculo_c(C, n); /* funcion calculo c */

    for (i = 0; i < n; i++)
    { /* primer bucle for */
        s1 = 0;
        for (j = 0; j < n; j++)
            s1 += A[i] * B[i];
        

        for (j = 0; j < n; j++)
            A[i] *= s1;
    }
    for (i = 0; i < n; i++)
    { /* segundo bucle for */
        s2 = 0;
        for (j = 0; j < n; j++)
            s2 += B[i] * C[i];
        for (j = 0; j < n; j++)
            C[i] *= s2;
    }
    /* calculo final */
    a = s1 / s2;
    res = 0;
    for (i = 0; i < n; i++)
        res += a * C[i];
    return res;
}

double OpArr_parallel(double A[], double B[], double C[], int n)
{
    int i, j;
    double s1, s2, a, res;
    
    #pragma omp parallel private(i,j) shared(s1,s2,a,res)
    {
        #pragma omp master
        {
            #pragma omp task depend(out:A[0:n])
                calculo_a(A, n); /* funcion calculo a */
            #pragma omp task depend(out:B[0:n])    
                calculo_b(B, n); /* funcion calculo b */
            #pragma omp task depend(out:C[0:n]) 
                calculo_c(C, n); /* funcion calculo c */

                    

            #pragma omp task depend(in: A[0:n],B[0:n]) depend(out: s1)
            for (i = 0; i < n; i++)
            { /* primer bucle for */
                s1 = 0;
                for (j = 0; j < n; j++)
                    s1 += A[i] * B[i];

                for (j = 0; j < n; j++)
                    A[i] *= s1;
            }

            #pragma omp task depend(in:B[0:n], C[0:n]) depend(out:C[0:n], s2)
            for (i = 0; i < n; i++)
            { /* segundo bucle for */
                s2 = 0; 
                for (j = 0; j < n; j++)
                    s2 += B[i] * C[i];
                for (j = 0; j < n; j++)
                    C[i] *= s2;
            }
            /* calculo final */
            #pragma omp task depend(in: C[0:n], s1, s2)
            {
                    a = s1 / s2;
                    res = 0;
                    for (i = 0; i < n; i++)
                        res += a * C[i];
            }
        }
    }
    return res;
}


int main(){
    int n = 20;
    double A[n], B[n], C[n];
    double result = OpArr(A, B, C, n);
    restart(A, B, C, n);
    double result_parallel = OpArr_parallel(A, B, C, n);
    cout << "Resultado final: \t\t" << result <<  endl;
    cout << "Resultado final parallel: \t" << result_parallel << endl;
    return 0;
}
