#include <stdio.h>
#include <stdlib.h>

#define SIZE (1024L)
#define ITER 2

int run_xor_test( const char* A, const int _NITER, const unsigned long _MEMSIZE );
int run_matrix_test( const char* A, const int _NITER, const unsigned long _MEMSIZE );

int main() {
    char* A = (char*) malloc(SIZE);
    int (*foo)(const char*, const int, const unsigned long);
    foo = run_matrix_test;
    return foo( A, ITER, SIZE);
}
    
