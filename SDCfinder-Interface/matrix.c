// TODO ENSURE THAT VALUES OF _BUFFER DO NOT CHANGE.
#include <sdcmf.h>

static unsigned long long N;
static int NITER;

int	**A;//[N][N] = {0};
int	**B;//[N][N] = {0};
int	**C;//[N][N] = {0};

void init( void ) {
  int i, j, v=0;
  for(j=0;j<N;j++) {
    for(i=0;i<N;i++) A[j][i] = v++;
    B[j][j] = 1;
  }
}

void mul( void ) {
  int i, j, k, x;
  for(k=0;k<N;k++) {
    for(j=0;j<N;j++) {
      x = 0;
      for(i=0;i<N;i++) x += A[k][i] * B[j][i];
      C[k][j] = x;
    }
  }
}

int chk( void ) {
  int i, j, v=0;
  for(j=0;j<N;j++) {
    for(i=0;i<N;i++) {
      if( C[j][i] != v++ ) {
	printf( "C[%d][%d]=%d !!!!\n", j, i, C[j][i] );
	return 0;
      }
    }
  }
  return 1;
}

#include <stdio.h>
void prt( int **x, char *name ) {
  if( N < 10 ) {
    int i, j;
    for( j=0; j<N; j++ ) {
      printf( "%s[%d] %d", name, j, x[j][0] );
      for( i=1; i<N; i++ ) {
	printf( " ,%d", x[j][i] );
      }
      printf( "\n" );
    }
  }
}

int run_matrix_test( char* _BUFFER, const int _NITER, const unsigned long long _MEMSIZE ) {
  int iter;

  N = 1;
  while ((3*N*N) < _MEMSIZE) {
      N++;
  }
  A[0] = (int*) _BUFFER;
  B[0] = (int*) (_BUFFER + N);
  C[0] = (int*) (_BUFFER + 2*N);

  NITER = _NITER;

  init();
  for( iter=0; iter<NITER; iter++ ) {
    sdcmf_input_data( A, sizeof(A) );
    sdcmf_input_data( B, sizeof(A) );
    sdcmf_input_data( C, sizeof(C) );
    sdcmf_input_end();

    mul();
    prt( A, "A" );
    prt( B, "B" );
    prt( C, "C" );

    sdcmf_output_data( C, sizeof(C) );
    sdcmf_output_end();

    sdcmf_appcheck( chk() );
  }
  return 0;
}
