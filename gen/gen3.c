#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

/* cc gen3.c -lfrm -lm -o gen3 */

# define SIZE 1024

main(narg, argv) int 
	narg;
	char           *argv[]; {
	float           tcoef[SIZE], vcoef[SIZE], factor, *f, *ff, scale = 0.0;
	float           expr();
	extern int      exprerr;
	int             i, j, seglen, length, nc, closed = 1;

	if (narg < 4 || *argv[1] != '-' || *(argv[1] + 1) != 'L') {
		fprintf(stderr, "Usage: gen3 -Llength v1 v2 ... vN\n");
		exit(-1);
	}
	length = expr(argv[1] + 2);
	if (exprerr) {
		fprintf(stderr, "GEN3: Illegal length expression\n");
		exit(-1);
	}
	j = 2;
	if (!strcmp(argv[2], "-o")) {
		closed = 0;
		j = 3;
	}
	if (*argv[2] == '-' && !(index("0123456789.", *(argv[2] + 1))))
		j = 3;
	for (nc = 0; j < narg; nc++) {
		vcoef[nc] = expr(argv[j++]);
		if (exprerr) {
			fprintf(stderr, "gen3: expression error: %s\n", argv[j - 1]);
			exit(1);
		}
		if (fabs(vcoef[nc]) > scale)
			scale = fabs(vcoef[nc]);
	}
	for (i = 0; i < nc; i++)
		tcoef[i] = (float) i *(length - closed) / (nc - 1);

	for (
	     ff = f = (float *) malloc(length * sizeof(float)), i = 0;
	     i < nc - 1;
	     f += seglen - 1, i++) {
		seglen = floor(tcoef[i + 1] +.5) - floor(tcoef[i] +.5) + 1;
		trans(vcoef[i], 0., vcoef[i + 1], seglen, f);
	}
	if (isatty(1)) {
		for (i = 0; i < length; i++)
			printf("%f\n", *(ff + i));
	} else {
		for (i = 0; i < length; i++) {
			putfloat((ff + i));
		}
	}
	flushfloat();
	exit(0);		/* Return status of 0 if all went OK */
}
