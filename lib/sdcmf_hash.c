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
  MD5_CTX	sdcfm_hash_ctx;

  if( MD5_Init( &sdcfm_hash_ctx ) == 0 ) {
    /* error */
  }
  if( MD5_Update( &sdcfm_hash_ctx, buff->data, buff->currp  ) == 0 ) {
    /* error */
  }
  if( MD5_Final( (unsigned char*) &buff->hash, &sdcfm_hash_ctx  ) == 0 ) {
    /* error */
  }
}

void sdcmf_hash_check( sdcmf_data_t *buff ) {
  MD5_CTX	sdcfm_hash_ctx;
  sdcmf_hash_t	hash;

  if( MD5_Init( &sdcfm_hash_ctx ) == 0 ) {
    /* error */
  }
  if( MD5_Update( &sdcfm_hash_ctx, buff->data, buff->currp  ) == 0 ) {
    /* error */
  }
  if( MD5_Final( (unsigned char*) &hash, &sdcfm_hash_ctx  ) == 0 ) {
    /* error */
  }
  if( memcmp( &hash, &buff->hash, sizeof(sdcmf_hash_t) ) != 0 ) {
    /* hash error */
  }
}
