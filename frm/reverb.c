#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

main(narg,argv) int narg; char *argv[]; {

    register int i, j, delay, iter;
    register float *buf, scale;
    float input, output, gain;
    float thresh = 1e-6; /* cutoff threshhold for reverb tail (-60dB) */

    if (narg < 3) {
	fprintf(stderr,"Usage: comb gain delay\n");
	exit(1);
    }
    gain = expr(argv[1]);
    if (exprerr) {
	fprintf(stderr, "Illegal gain expression '%s'\n",argv[1]); 
	exit(1);
    }
    if (fabs( (double) gain) >= 1.0) {
	fprintf(stderr,"Illegal gain = %f\n",gain);
	exit(1);
    }
    delay = expr(argv[2]);
    if (exprerr) {
	fprintf(stderr, "Illegal delay expression '%s'\n",argv[1]); 
	exit(1);
    }
    if (delay<1 || delay>100000) {
	fprintf(stderr,"Illegal delay = %d\n",delay);
	exit(1);
    }
    if (isatty(0)) {
	fprintf(stderr,"comb input must be a file or pipe.\n");
	exit(1);
    }
    buf = (float *) calloc(delay, sizeof(float)); /* allocate delay buffer */
    i = -1;
    scale = 1./(1.+gain);
    if (isatty(1)) { /* output in ASCII on screen */
	while ( getfloat(&input) > 0 ) {
	    if (++i >= delay)
		i -= delay;
	    output = *(buf+i) + input;
	    printf("%f\n",output);
	    *(buf+i) = *(buf+i) * gain - input * scale;
	}
	iter = delay * log( (double) thresh)/log( (double) gain);
	for (j = 0; j < iter; j++) {
	    if (++i >= delay)
		i -= delay;
	    printf("%f\n",*(buf+i));
	    *(buf+i) = *(buf+i) * gain;
	}
    } else { /* output in binary to stdout */
	while ( getfloat(&input) > 0 ) {
	    if (++i >= delay)
		i -= delay;
	    output = *(buf+i) - input;
	    putfloat( &output );
	    *(buf+i) = *(buf+i) * gain + input * scale;
	}
	iter = delay * log( (double) thresh)/log( (double) gain);
	for (j = 0; j < iter; j++) {
	    if (++i >= delay)
		i -= delay;
	    putfloat( buf+i );
	    *(buf+i) = *(buf+i) * gain;
	}
	flushfloat();
    }
    exit(0);
}
