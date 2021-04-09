#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

main(narg,argv) 
	int narg; 
	char *argv[]; 
{

    register long i, n = 1024 * 4, delay = 0;
    float one = 1.0, zero = 0.0;

    if (narg > 3 || argv[1][0] == '-') {
	fprintf(stderr, "Usage: impulse length (in samples)\n");
	exit(1);
    } else if (narg == 3){
	    delay = expr(argv[2]);
	    n = expr(argv[1]) - delay;
    } else if (narg == 2) 
	    n = expr(argv[1]);
    if (isatty(1)) {
	for (i = 1; i <= delay; i++) printf("0.000000\n");
	printf("1.000000\n");
	for (i = 1; i < n; i++) printf("0.000000\n");
    } else {
	for (i = 1; i <= delay; i++) putfloat(&zero);
	putfloat(&one);
	for(i = 1; i < n; i++) putfloat(&zero);
    }
    flushfloat();
    exit(0);
}
