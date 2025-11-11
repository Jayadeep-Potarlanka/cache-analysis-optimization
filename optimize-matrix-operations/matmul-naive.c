#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>

#define N 1000
#define in(i,j) ((i*N)+j)

void matmulnaive(int *a, int *b, int *c) {
    int i, j, k;
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            for(k = 0; k < N; k++) {
                c[in(i,j)] += a[in(i,k)] * b[in(k,j)];
            }
        }
    }
}

int main() {
    int *a = (int*) malloc(N*N*sizeof(int));
    int *b = (int*) malloc(N*N*sizeof(int));
    int *c = (int*) calloc(N*N,sizeof(int));
    int i;

    for(i = 0; i < N*N; i++) {
        a[i] = drand48() * 2 - 1;
        b[i] = drand48() * 2 - 1;
    }

    struct timeval start, end;  
    gettimeofday(&start, NULL);
    matmulnaive(a, b, c);
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    double gflops = 2.0 * N * N * N / (seconds * 1e9);
    printf("Achieved GFLOPS is %lf\n", gflops);

    free(a); free(b); free(c);
    return 0;
}
