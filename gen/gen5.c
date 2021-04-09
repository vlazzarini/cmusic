#include <stdio.h>
#include <math.h>
#include <carl/carl.h>
#define TWOPI (8.*atan(1.))

/* cc gen5.c -lfrm -lm -o gen5 */

# define SIZE 1024

main(narg, argv) int 
	narg;
	char           *argv[]; {
	float           coef[SIZE], factor, *f, *ff, scale = 0.0;
	float           harm, amp, pha;
	float           expr();
	extern int      exprerr;
	int             i, j, seglen, length, na, nc, closed = 0;

	if (narg < 5 || *argv[1] != '-' || *(argv[1] + 1) != 'L') {
		fprintf(stderr, "Usage: gen5 -Llength h1 a1 p2 ... hN aN pN\n");
		exit(-1);
	}
	length = expr(argv[1] + 2);
	if (exprerr) {
		fprintf(stderr, "GEN5: expr = %s\n", argv[1] + 2);
		fprintf(stderr, "GEN5: Illegal length expression\n");
		exit(-1);
	}
	ff = f = (float *) calloc(length, sizeof(float));
	i = 2;
	if (!strcmp(argv[2], "-c")) {
		closed = 1;
		i = 3;
	}
	if (*argv[2] == '-' && !(index("0123456789.", *(argv[2] + 1))))
		i = 3;
	for (nc = 0; i < narg; nc++) {
		harm = expr(argv[i++]);
		amp = expr(argv[i++]);
		pha = expr(argv[i++]);
		factor = harm * TWOPI / (length - closed);
		for (f = ff, j = 0; j < length; j++, f++) {
			*f += amp * sin(j * factor + pha);
			if (fabs(*f) > scale)
				scale = fabs(*f);
		}
	}
	for (scale = 0.0, f = ff, j = 0; j < length; j++, f++)
		if (fabs(*f) > scale)
			scale = fabs(*f);
	if (scale == 0.0) {
		fprintf(stderr, "GEN5: all-zero function\n");
		exit(-1);
	}
	if (isatty(1)) {
		for (i = 0; i < length; i++)
			printf("%f\n", *(ff + i));
	} else {
		for (i = 0; i < length; i++)
			putfloat((ff + i));
	}
	flushfloat();
	exit(0);		/* Return status of 0 if all went OK */
}
