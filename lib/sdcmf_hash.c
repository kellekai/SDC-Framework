/*
  * $RIKEN_copyright:$
  * SDCMF_VERSION:$
  * $SDCMF_license:$
*/
/*
  * Written by Atsushi HORI <ahori@riken.jp>, 2018
*/

#include <string.h>
#include <sdcmf_internal.h>

void sdcmf_hash_create( sdcmf_data_t *buff ) {
  MD5_CTX	hash_ctx;

  if( buff->currp > 0 ) {
    if( MD5_Init( &hash_ctx )                                == 0 ) ABRT;
    if( MD5_Update( &hash_ctx, buff->data, buff->currp )     == 0 ) ABRT;
    if( MD5_Final( (unsigned char*) &buff->hash, &hash_ctx ) == 0 ) ABRT;
  }
}

int sdcmf_hash_check( sdcmf_data_t *buff ) {
  MD5_CTX	hash_ctx;
  sdcmf_hash_t	hash;

  if( buff->currp > 0 ) {
    if( MD5_Init( &hash_ctx )                              == 0 ) ABRT;
    if( MD5_Update( &hash_ctx, buff->data, buff->currp  )  == 0 ) ABRT;
    if( MD5_Final( (unsigned char*) &hash, &hash_ctx  )    == 0 ) ABRT;
    if( memcmp( &hash, &buff->hash, sizeof(sdcmf_hash_t) ) != 0 ) return -1;
  }
  return 0;
}
