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
static int	sdcmf_sleep;
static double	sdcmf_start;

double	sdcmf_accum_sleep;
double	sdcmf_accum_elapsed;

static double get_time( void ) {
  struct timeval tv;
  gettimeofday( &tv, NULL );
  return ((double)tv.tv_sec) + ((double)tv.tv_usec*1e-6);
}

static void sdcmf_copyin_data( void *data, size_t size, sdcmf_data_t *buff ) {
  if( buff->size - buff->currp < size ) {
    buff->size += ( size < 1024 ) ? 1024 : size;
    buff->data = realloc( buff->data, buff->size );
    if( buff->data == NULL ) ABRT;
  }
  if( buff->orgp_n == buff->orgp_c ) {
    buff->orgp_n += SDCMF_NUM_ORGP;
    buff->orgp = realloc( buff->orgp, buff->orgp_n * sizeof(sdcmf_orgp_t) );
    if( buff->orgp == NULL ) ABRT;
  }
  memcpy( buff->data + buff->currp, data, size );
  buff->currp += size;

  buff->orgp[buff->orgp_c].p  = data;
  buff->orgp[buff->orgp_c].sz = size;
  buff->orgp_c ++;
}

static int sdcmf_check_data( sdcmf_data_t *buff ) {
  int i, j;

  for( i=0,j=0; i<buff->orgp_c; i++ ) {
    if( memcmp( &buff->data[j], buff->orgp[i].p, buff->orgp[i].sz ) != 0 ) {
      return -1;
    }
    j += buff->orgp[i].sz;
  }
  return 0;
}

static void sdcmf_sleep_time( void ) {
  static int	factor = 0;
  char		*opt;

  if( ( opt = getenv( "SDCMF_RATIO" ) ) != NULL ) {
    factor = atoi( opt );
  }
  if( factor == 0 ) factor = SDCMF_DEFAULT_FACTOR;
  sdcmf_sleep = ((int)( get_time() - sdcmf_start )) * factor;
  if( sdcmf_sleep < 1 ) sdcmf_sleep = 10;
#ifdef VERBOSE
  fprintf( stderr, "SLEEP %d sec\n", sdcmf_sleep );
#endif
}

static void sdcmf_init( void ) {
  static int started = 0;
  if( !started ) {
    started = 1;
    sdcmf_start = get_time();
    memset( (void*) &sdcmf_input,  0, sizeof(sdcmf_input)  );
    memset( (void*) &sdcmf_output, 0, sizeof(sdcmf_output) );
    sdcmf_start_message();
  }
}

void sdcmf_input_data( void *data, size_t size ) {
  sdcmf_init();
  if( sdcmf_iteration == 0 ) {
    sdcmf_copyin_data( data, size, &sdcmf_input );
  } else {
    memcpy( data, sdcmf_input.data + sdcmf_input.currp, size );
    sdcmf_input.currp += size;
    sdcmf_input.orgp[sdcmf_input.orgp_c].p  = data;
    sdcmf_input.orgp[sdcmf_input.orgp_c].sz = size;
    sdcmf_input.orgp_c ++;
  }
}

void sdcmf_input_end( void ) {
  sdcmf_init();
  if( sdcmf_iteration == 0 ) {
    sdcmf_hash_create( &sdcmf_input );
  } else {
    sleep( sdcmf_sleep );
    sdcmf_accum_sleep += ((double)sdcmf_sleep);
    sdcmf_accum_elapsed = get_time() - sdcmf_start;
    CHK( sdcmf_hash_check( &sdcmf_input ), "IH" );
    CHK( sdcmf_check_data( &sdcmf_input ), "ID" );
  }
  sdcmf_input.currp  = 0;
  sdcmf_input.orgp_c = 0;
}

void sdcmf_output_data( void *data, size_t size ) {
  sdcmf_init();
  if( sdcmf_iteration == 0 ) {
    sdcmf_copyin_data( data, size, &sdcmf_output );
  } else {
    sdcmf_output.currp += size;
    sdcmf_output.orgp[sdcmf_output.orgp_c].p  = data;
    sdcmf_output.orgp[sdcmf_output.orgp_c].sz = size;
    sdcmf_output.orgp_c ++;
  }
}

void sdcmf_output_end( void ) {
  sdcmf_init();
  if( sdcmf_iteration == 0 ) {
    sdcmf_hash_create( &sdcmf_output );
  } else {
    sdcmf_accum_elapsed = get_time() - sdcmf_start;
    CHK( sdcmf_hash_check( &sdcmf_output ), "OH" );
    CHK( sdcmf_check_data( &sdcmf_output ), "OD" );
  }
  sdcmf_output.currp  = 0;
  sdcmf_output.orgp_c = 0;
}

void sdcmf_appcheck( int ok ) {
  sdcmf_init();
  if( sdcmf_iteration == 0 ) {
    sdcmf_sleep_time();
  } else {
#ifdef VERBOSE
    fprintf( stderr, "ELAPSED %g sec (SLEEP %g sec)\n",
	     sdcmf_accum_elapsed, sdcmf_accum_sleep );
#endif
  }
  sdcmf_accum_elapsed = get_time() - sdcmf_start;
  CHK( !ok, "AP" );
  CHK( sdcmf_hash_check( &sdcmf_input ), "IH" );
  CHK( sdcmf_hash_check( &sdcmf_output ), "OH" );
  sdcmf_iteration ++;
}
