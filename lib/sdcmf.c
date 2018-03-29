/*
  * $RIKEN_copyright:$
  * SDCMF_VERSION:$
  * $SDCMF_license:$
*/
/*
  * Written by Atsushi HORI <ahori@riken.jp>, 2018
*/

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sdcmf.h>
#include <sdcmf_internal.h>

static struct sdcmf_data	sdcmf_input;
static struct sdcmf_data	sdcmf_output;

static int 	sdcmf_iteration = 0;
static int	sdcmf_factor    = SDCMF_DEFAULT_FACTOR;
static double	sdcmf_first;
static double	sdcmf_start;
static double	sdcmf_sleep;
static double	sdcmf_accum_sleep;
static double	sdcmf_accum_elapsed;

static double get_time( void ) {
  struct timeval tv;
  gettimeofday( &tv, NULL );
  return ((double)tv.tv_sec) + ((double)tv.tv_usec*1e-6);
}

static void sdcmf_copyin_data( void *data, size_t size, sdcmf_data_t *buff ) {
  if( buff->size - buff->currp > size ) {
    if( ( buff->data = realloc( buff->data, size ) ) == NULL ) {
      /* ENOMEM */
    }
    buff->size += size;
  }
  if( buff->orgp_n - buff->orgp_c == 0 ) {
    buff->orgp_n += SDCMF_NUM_ORGP;
    buff->orgp = realloc( buff->orgp, buff->orgp_n * sizeof(sdcmf_orgp_t) );
    if( buff->orgp == NULL ) {
      /* ENOMEM */
    }
  }
  memcpy( buff->data + buff->currp, data, size );
  buff->currp += size;

  buff->orgp[buff->orgp_c].p  = data;
  buff->orgp[buff->orgp_c].sz = size;
  buff->orgp_c ++;
}

static void sdcmf_copyout_data( sdcmf_data_t *buff, void *data, size_t size ) {
  memcpy( data,  buff->data + buff->currp, size );
  buff->currp += size;
}

static int sdcmf_check_data( sdcmf_data_t *buff ) {
  int i, j;
  for( i=0,j=0; i<buff->orgp_c; i++ ) {
    if( memcmp( &buff->data[j],
		buff->orgp[i].p,
		buff->orgp[i].sz ) != 0 ) {
      /* error */
    }
    j += buff->orgp[i].sz;
  }
}

static void sdcmf_sleep_time( void ) {
  char *opt;
  if( ( opt = getenv( "SDCMF_RATIO" ) ) != NULL ) {
    sdcmf_factor = atoi( opt );
    if( sdcmf_factor == 0 ) sdcmf_factor = SDCMF_DEFAULT_FACTOR;
  }
}

void sdcmf_input_data( void *data, size_t size ) {
  if( sdcmf_iteration == 0 ) {
    if( sdcmf_input.size == 0 ) {
      sdcmf_start_message();
      memset( (void*) &sdcmf_input,  0, sizeof(sdcmf_input)  );
      sdcmf_first = get_time();
    }
    sdcmf_copyin_data( data, size, &sdcmf_input );
    sdcmf_sleep_time();
  } else {
    sdcmf_start = get_time();
    if( sdcmf_input.currp == 0 ) {
      sdcmf_hash_check( &sdcmf_input );
    }
    sdcmf_copyout_data( &sdcmf_input, data, size );
  }
}

void sdcmf_input_end( void ) {
  if( sdcmf_iteration == 0 ) {
    sdcmf_hash_create( &sdcmf_input );
  } else {

    sleep( sdcmf_sleep );
    sdcmf_accum_sleep += ((double)sdcmf_sleep);
    sdcmf_hash_check( &sdcmf_input );
    sdcmf_check_data( &sdcmf_input );
  }
  sdcmf_input.currp = 0;
}

void sdcmf_output_data( void *data, size_t size ) {
  if( sdcmf_iteration == 0 ) {
    if( sdcmf_output.size == 0 ) {
      memset( (void*) &sdcmf_output, 0, sizeof(sdcmf_output) );
    }
    sdcmf_copyin_data( data, size, &sdcmf_input );
  }
}

void sdcmf_output_end( void ) {
  if( sdcmf_iteration == 0 ) {
    sdcmf_hash_create( &sdcmf_output );
  } else {
    sdcmf_hash_check( &sdcmf_output );
    sdcmf_check_data( &sdcmf_output );
  }
  sdcmf_output.currp = 0;
}

void sdcmf_appcheck( int ok ) {
  if( sdcmf_iteration == 0 ) {
    sdcmf_sleep = ((int)( get_time() - sdcmf_first )) * sdcmf_factor;
  } else {
    sdcmf_accum_elapsed += get_time() - sdcmf_start;
  }
  sdcmf_iteration ++;
}
