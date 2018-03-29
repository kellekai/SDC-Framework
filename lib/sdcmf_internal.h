/*
  * $RIKEN_copyright:$
  * SDCMF_VERSION:$
  * $SDCMF_license:$
*/
/*
  * Written by Atsushi HORI <ahori@riken.jp>, 2018
*/

#include <openssl/md5.h> /* -lcrypt */

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

void sdcmf_hash_create( sdcmf_data_t *buff );
void sdcmf_hash_check( sdcmf_data_t *buff );
