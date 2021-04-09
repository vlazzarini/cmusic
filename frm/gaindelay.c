#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

main(narg,argv) 
	int narg; char *argv[]; 
{

    register int i, j, delay;
    register float *buf;
    float input, output, gain;
    extern exprerr;

    if (narg < 3) {
	fprintf(stderr,"Usage: gaindelay gain delay\n");
	exit(1);
    }
    gain = expr(argv[1]);
    if (exprerr) {
	fprintf(stderr, "Illegal gain expression '%s'\n",argv[1]); 
	exit(1);
    }
    delay = expr(argv[2]);
    if (exprerr) {
	fprintf(stderr, "Illegal gain expression '%s'\n",argv[1]); 
	exit(1);
    }
    if (delay<1 || delay>100000) {
	fprintf(stderr,"Illegal delay = %d\n",delay);
	exit(1);
    }
    if (isatty(0)) {
	fprintf(stderr,"gaindelay input must be a file or pipe.\n");
	exit(1);
    }
    buf = (float *) calloc(delay, sizeof(float)); /* allocate delay buffer */
    i = -1;
    if (isatty(1)) {
	while ( getfloat(&input) > 0) {
	    if (++i >= delay)
		i -= delay;
	    printf("%f\n", *(buf+i)); 
	    *(buf+i) = gain * input;
	}
	for (j = 1; j < delay; j++) {
	    if (++i >= delay)
		i -= delay;
	    printf("%f\n", *(buf+i)); 
	}
    } else {
	while ( getfloat(&input) > 0) {
	    if (++i >= delay)
		i -= delay;
	    putfloat( (buf+i) );
	    *(buf+i) = gain * input;
	}
	for (j = 1; j < delay; j++) {
	    if (++i >= delay)
		i -= delay;
	    putfloat( (buf+i) );
	}
    flushfloat();
    }
    exit(0);
}
