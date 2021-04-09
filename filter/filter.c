#include <math.h>
#include <stdio.h>
#include <carl/sndio.h>
#include <carl/carl.h>
#include <carl/josprogs.h>

#define NCMAX 1024

/*
 * filter.c
 * 
 * This program takes floatsams on stdin and applies an nz-zero/np-pole 
 * filter where the filter is defined by a standard format filter_file 
 * specified on the command line.  The output is floatsams to stdout.
 * 
 * To compile:    cc -O filter.c -ljos -lcarl [-lnm] -lm
 *
 * original program by jos
 * revised by dolson
 * latest revision: 15 May 87 - frm
 */

main( argc, argv ) int argc ; char **argv ; {
 float	*ibuf, *obuf, ic[NCMAX], oc[NCMAX] ;
 int	nz, np, nowz, nowp ;
 char	ch, fname[NAMSIZ] ;
 FILE	*fp ;
    if ( isatty(0) )
	usage(1) ;
/*
 * Interpret commandline by calling dgl's subroutine crack
 */
    while ( (ch = crack( argc, argv, "h", 0 ) ) != NULL ) {
        switch ( ch ) {
            case 'h':  usage(0) ;
            default:   usage(1) ;    /* this exits with error */
        }
    }
    if ( arg_index < argc ) fp = fopen( argv[arg_index], "r" ) ;
    else {
	fprintf( stderr, "filter: no filter_file\n" ) ;
	exit(1) ;
    }
    strcpy( fname, argv[arg_index] ) ;
/*
 * Read in filter parameters
 */
    if ( !getflt( fname, "test.flt", NCMAX, &nz, &np, ic, oc ) ){
        fprintf( stderr, "filter: bad filter_file\n" ) ;
        exit(1) ;
    }
/*
 * Set up buffers
 */
    if ( (ibuf = (float *) calloc( nz, sizeof(float) ) ) == NULL )
        malerr( "filter: insufficient input buffer memory", 1 ) ;
    if ( ( obuf = (float *) calloc( np, sizeof(float) ) ) == NULL )
        malerr( "filter: insufficient output buffer memory", 1 ) ;
/*
 * Main loop: THE general digital filter
 */
    for( nowz = nowp = 0 ; getfloat( ibuf + nowz ) > 0 ; ) {

     register float *op, *pp, *p, *coef ;
     float *endi = ibuf + nz - 1, *endo = obuf + np - 1, *ocbeg = oc + 1 ;

	*( op = obuf + nowp ) = 0.0 ;

	for ( pp = p = ibuf + nowz, coef = ic ; p >= ibuf ; )
            *op += *coef++ * *p-- ;
	for ( p = endi ; p > pp ; )
            *op += *coef++ * *p-- ;
	if ( ++nowz >= nz ) 
	    nowz = 0 ;
        
        for ( pp = p = op - 1, coef = ocbeg ; p >= obuf ; )
            *op += *coef++ * *p-- ;
        for ( p = endo ; p > pp ; )
            *op += *coef++ * *p-- ;
	if ( ++nowp >= np )
	    nowp = 0 ;

        putfloat( op ) ;
    }

    flushfloat() ;

    exit(0) ;
}

usage(exitcode)
    int exitcode ;
{
    fprintf(stderr,"usage: filter filter_file < floatsams > floatsams \n") ;
    exit(exitcode) ;
}

malerr(str, ex)
    char *str ;
    int ex ;
{
    fprintf(stderr, "%s\n", str) ;
    exit(ex) ;
}
