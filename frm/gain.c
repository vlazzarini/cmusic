#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

main(narg,argv) 
	int narg; 
	char *argv[]; 
{
    float input, output;
    float gain;

    if (narg != 2) {
	fprintf(stderr, "usage: gain factor\n");
	exit(1);
    } else 
	gain = expr(argv[1]);
    if (exprerr) {
	fprintf(stderr, "Illegal gain expression '%s'\n",argv[1]); 
	exit(1);
    }
    if (isatty(0)) {
	fprintf(stderr,"input must be a file or pipe.\n");
	exit(1);
    }
    if (isatty(1)) {
	while ( getfloat(&input) > 0) {
	    printf("%f\n",input*gain); 
	}
    } else {
	while ( getfloat(&input) > 0) {
	    output = input*gain;
	    putfloat(&(output));
	}
	flushfloat();
    }
    exit(0);
}


