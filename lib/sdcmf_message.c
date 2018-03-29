/*
  * $RIKEN_copyright:$
  * SDCMF_VERSION:$
  * $SDCMF_license:$
*/
/*
  * Written by Atsushi HORI <ahori@riken.jp>, 2018
*/

#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sdcmf_internal.h>

extern double	sdcmf_accum_sleep;
extern double	sdcmf_accum_elapsed;

static FILE	*fp = NULL;
static int	nerrors;

#define TS_STRLEN	(64)

static char	ts_str[TS_STRLEN];

static void open_file( void ) {
  if( fp == NULL ) {
    char	*opt;
    char	*filename = NULL;

    if( ( opt = getenv( "SDCMF_FILENAME" ) ) != NULL ) {
      filename = opt;
    }
    if( filename != NULL && filename[0] != '\0' ) {
      if( ( fp = fopen( filename, "a" ) ) == NULL ) {
	fp = stderr;
      }
    } else {
      fp = stderr;
    }
  }
}

static char *time_stamp( void ) {
  time_t 	ts = time( NULL );
  struct tm	*tmp = localtime( &ts );
  strftime( ts_str, TS_STRLEN, "%F %T", tmp );
}

void sdcmf_start_message( void ) {
  open_file();
}

void sdcmf_success( char *id ) {
  open_file();
  time_stamp();
  fprintf( fp, "%s Success(%s), %d, %g, %g\n",
	   ts_str, id, nerrors, sdcmf_accum_sleep, sdcmf_accum_elapsed );
  fflush( fp );
  fsync( fileno( fp ) );
}

void sdcmf_fail( char *id ) {
  open_file();
  nerrors ++;
  time_stamp();
  fprintf( fp, "%s Failure(%s), %d, %g, %g\n",
	   ts_str, id, nerrors, sdcmf_accum_sleep, sdcmf_accum_elapsed );
  fflush( fp );
  fsync( fileno( fp ) );
}
