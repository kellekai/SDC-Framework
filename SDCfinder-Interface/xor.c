
#include <sdcmf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/md5.h>

// parameters
static unsigned long N;
static unsigned long MEMSIZE;
static int NITER;

uint32_t* A;
unsigned char hash_A_raw[MD5_DIGEST_LENGTH];
unsigned char hash_A_processed[MD5_DIGEST_LENGTH];

int init() {
    
    if (A==NULL) {
        fprintf(stderr, "INIT: [FAILURE] nullptr passed!\n");
        return -1;
    }

    srand(time(NULL));
    
    unsigned long i;
    for(i=0; i<N; ++i) {
        A[i] = rand();
    }
   
    if ( MD5((unsigned char*) A, MEMSIZE, hash_A_raw) == NULL ) {
        printf("ERROR CREATING HASH IN INIT\n");
        free(A);
        return -1;
    } else {
        return 0;
    }
}

void mem_stress() {
    // -> MODIFY (i := 0 -> N-1) : A'[i] = A[i] XOR A[i+1]
    // NOTE : A'[N-1] = A[N-1] XOR A'[0] 
    unsigned long i;
    for(i=0; i<N; ++i) {
        A[i] = A[i]^A[(i+1)%((unsigned long)N)];
    }
    
    // -> INVERSE (i := N-1 -> 0) : A[i] = A'[i] XOR A[i+1]
    // NOTE : A[N-1] = A'[N-1] XOR A'[0]
    for(i=0; i<N; ++i) {
        A[N-1-i] = A[N-1-i]^A[(N-1-i+1)%((unsigned long)N)];
    }
}

int chk() {
    
    if ( MD5((unsigned char*) A, MEMSIZE, hash_A_processed) == NULL ) {
        printf("ERROR CREATING HASH IN CHK\n");
        return 0;
    }
    else if ( memcmp(hash_A_raw,hash_A_processed,MD5_DIGEST_LENGTH) == 0 ) {
        return 1;
    } else {
        return 0;
    }
}

int run_xor_test( const char* _A, const int _NITER, const unsigned long _MEMSIZE ) {
    
    A = (uint32_t*) _A;
    N = _MEMSIZE/sizeof(uint32_t);
    MEMSIZE = N*sizeof(uint32_t);
    NITER = _NITER;


    int iter;

    if ( init() == -1 ) {
        return -1;
    }
    printf("INIT: [SUCCESS] array of %ld bytes randomly initialized.\n", MEMSIZE);
    for( iter=0; iter<NITER; iter++ ) {
        printf("    ITERATION: %d\n",iter);
        sdcmf_input_data( A, MEMSIZE );
        sdcmf_input_end();

        mem_stress();

        sdcmf_output_data( A, MEMSIZE );
        sdcmf_output_end();
        
        sdcmf_appcheck( chk() );
    }

    free(A);
    if (sdcmf_errors() == 0) {
        printf("FINALIZE: [SUCCESS] no errors occured.\n");
    } else {
        printf("FINALIZE: [WARNING] errors occured! errors: %d\n", sdcmf_errors());
    }


    return 0;
}
