/*
  * $RIKEN_copyright:$
  * SDCMF_VERSION:$
  * $SDCMF_license:$
*/
/*
  * Written by Atsushi HORI <ahori@riken.jp>, 2018
*/

#include <openssl/md5.h> /* -lcrypt */
#include <stdlib.h>
#include <stdio.h>

#define SDCMF_DEFAULT_FACTOR	(10)
#define SDCMF_NUM_ORGP		(16)

typedef struct orgp {
  void 		*p;
  size_t 	sz;
} sdcmf_orgp_t;

typedef struct {char x[MD5_DIGEST_LENGTH];}	sdcmf_hash_t;

typedef struct sdcmf_data {
  char 		*data;
  size_t	size;
  size_t	currp;
  sdcmf_hash_t	hash;
  int		orgp_n;
  int		orgp_c;
  sdcmf_orgp_t 	*orgp;
} sdcmf_data_t;

#define ABRT	\
  do {fprintf( stderr, "%s %s():%d\n", __FILE__, __func__, __LINE__ ); \
    exit(9);} while(0)

//#define VERBOSE
#ifndef VERBOSE
#define CHK(X,Y)							\
  do { if((X)) sdcmf_fail(Y); else sdcmf_success(Y); } while(0)
#else
#define CHK(X,Y)							\
  do {if((X)) {fprintf(stderr,"%s %s():%d [%s] FAILED !!!!\n", 		\
		       __FILE__,__func__,__LINE__,#X);sdcmf_fail(Y);}	\
    else {fprintf(stderr,"%s %s():%d [%s] succeeded\n",			\
		  __FILE__,__func__,__LINE__,#X);sdcmf_success(Y);}} while(0)
#endif

/* sdcmf_hash.c */
void sdcmf_hash_create( sdcmf_data_t *buff );
int  sdcmf_hash_check( sdcmf_data_t *buff );

/* sdcmf_message.c */
void sdcmf_start_message( void );
void sdcmf_success( char* );
void sdcmf_fail( char* );
