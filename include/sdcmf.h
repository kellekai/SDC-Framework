/*
  * $RIKEN_copyright:$
  * SDCMF_VERSION:$
  * $SDCMF_license:$
*/
/*
  * Written by Atsushi HORI <ahori@riken.jp>, 2018
*/

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

  void sdcmf_input_data( void *data, size_t size );
  void sdcmf_input_end( void );
  void sdcmf_output_data( void *data, size_t size );
  void sdcmf_output_end( void );
  void sdcmf_appcheck( int ok );

#ifdef __cplusplus
}
#endif
