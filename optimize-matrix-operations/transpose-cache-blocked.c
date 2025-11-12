#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>

#define N 1000
#define in(i,j) ((i*N)+j)

void transpose_blocking(int *src, int *dst, int width, int blocksize) {
    int i, j, x, y;
    int src_i, dst_i;
    for (i = 0; i < width; i += blocksize) {
        for (j = 0; j < width; j += blocksize) {
            for (x = i; x < i + blocksize && x < width; ++x) {
                for (y = j; y < j + blocksize && y < width; ++y) {
                    src_i = x * width + y;
                    dst_i = y * width + x;
                    dst[dst_i] = src[src_i];
                }
            }
        }
    }
}
void transpose_naive(int n, int blocksize, int *dst, int *src) { 
 for (int x = 0; x < n; x++) { 
   for (int y = 0; y < n; y++) { 
       dst[y + x * n] = src[x + y * n]; 
 } 
 } 
} 

int main() {
    int *a = (int*) malloc(N*N*sizeof(int));
    int *b = (int*) malloc(N*N*sizeof(int));
    int i;

    for(i = 0; i < N*N; i++) {
        a[i] = drand48() * 2 - 1;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);
   transpose_naive(N, 5000, b,a);
    gettimeofday(&end, NULL);
    double seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    double gflops = 2e-9 * N * N * N / seconds;
    printf("Achieved GFLOPS is %lf\n", gflops);
    
      gettimeofday(&start, NULL);
     transpose_blocking(a,b,N,5000);
    gettimeofday(&end, NULL);
    seconds = (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    gflops = 2e-9 * N * N * N / seconds;
    printf("Achieved GFLOPS is %lf\n", gflops);

    free(a); free(b);
    return 0;
}
