/* cc shepenv -lcarl -lm -o shepenv */
#include <stdio.h>
#include <carl/carl.h>
#include <math.h>

#define PI2 (8.*atan(1.))
#define logb(x,b) (log(x)/log(b))

main(argc, argv) int argc; char *argv[];  {
 double factor, cycles, base ;
 int len, i ;
 float value ;
    if(argc != 4 ) {
	fprintf(stderr, "shepenv -L cycles base\n") ;
	exit( -1 ) ;
    }
    len = expr( argv[1]+2 ) ;
    cycles = expr( argv[2] ) ;
    base = expr( argv[3] ) ;
    if(isatty(1)){
	for( i = 0; i < len; i++ )
	    printf("%d %f\n", i,
	    -.5*cos(
	    (PI2/cycles)*
	    log( 1 + ( (double) i/(len - 1))*( pow( base, cycles ) - 1 ) )/
	    log( base )
	    ) + .5
	    ) ;
    } else {
	factor = 1./(pow(base, cycles) - 1) ;
	for( i = 0; i < len; i++ ) {
	    value = -.5*cos(
	    (PI2/cycles)*
	    log( 1 + ( (double) i/(len - 1))*( pow( base, cycles ) - 1 ) )/
	    log( base )
	    ) + .5 ;
	putfloat( &value ) ;
	}
    }
    flushfloat();
    exit(0); /* Return status of 0 if all went OK */
}
