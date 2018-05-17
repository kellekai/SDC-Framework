#include <stdio.h>
#include <stdlib.h>

#define SIZE (1024L*1024L*1024L)
#define ITER 2

int run_xor_test( void* A, const int _NITER, const unsigned long long _MEMSIZE );

int main() {
    char* A = (char*) malloc(SIZE);
    int (*foo)(void*, const int, const unsigned long long);
    foo = run_xor_test;
    return foo( A, ITER, SIZE);
}
    
