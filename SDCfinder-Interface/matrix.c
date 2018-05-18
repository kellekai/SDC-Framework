#include <string.h>
#include <sdcmf.h>

static unsigned long N;
static int NITER;
unsigned long ASIZE;

char *BUFFER;
int *A, *B, *C;

void m_put(int *ptr, unsigned long i, unsigned long j, int val) {
    *(ptr+(i*N)+j) = val;
}

int m_get(int *ptr, unsigned long i, unsigned long j) {
    return *(ptr+(i*N)+j);
}

void m_init( const char* _BUFFER, const int _NITER, const unsigned long _MEMSIZE) { 
    N = 0;
    do {
        N++;
    } while ((3*N*N*sizeof(int)) < _MEMSIZE);
    N--;

    ASIZE = N*N*sizeof(int);
    
    printf("memsize: %ld, asize: %ld\n",_MEMSIZE, ASIZE);
    NITER = _NITER;
 
    A = (int*)(_BUFFER); 
    B = (int*)(_BUFFER + ASIZE); 
    C = (int*)(_BUFFER + 2*ASIZE); 
    memset(A, 0x0, ASIZE);
    memset(B, 0x0, ASIZE);
    memset(C, 0x0, ASIZE);

    int i, j, v=0;
    for(j=0;j<N;j++) {
        for(i=0;i<N;i++) m_put( A, j, i, v++ );
        m_put( B, j, j, 1 );
    }
}

void m_mul( void ) {
    int i, j, k, x;
    for(k=0;k<N;k++) {
        for(j=0;j<N;j++) {
            x = 0;
            for(i=0;i<N;i++) x += m_get( A, k, i ) * m_get( B, j, i );
            m_put( C, k, j, x );
        }
    }
}

int m_chk( void ) {
    int i, j, v=0;
    for(j=0;j<N;j++) {
        for(i=0;i<N;i++) {
            if( m_get( C, j, i ) != v++ ) {
                printf( "C[%d][%d]=%d !!!!\n", j, i, m_get( C, j, i ) );
                return 0;
            }
        }
    }
    return 1;
}

int run_matrix_test( const char* _BUFFER, const int _NITER, const unsigned long _MEMSIZE ) {
    int iter;

    m_init( _BUFFER, _NITER, _MEMSIZE );
    
    for( iter=0; iter<NITER; iter++ ) {
        sdcmf_input_data( A, ASIZE );
        sdcmf_input_data( B, ASIZE );
        sdcmf_input_data( C, ASIZE );
        sdcmf_input_end();

        m_mul();

        sdcmf_output_data( C, ASIZE );
        sdcmf_output_end();

        sdcmf_appcheck( m_chk() );
    }
    return 0;
}
