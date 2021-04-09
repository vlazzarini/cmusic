/* chubby - to compile this beastie utter: cc -O chubby.c -lsf -lm */
/* Author: Marc LeBrun, local implementor: Gareth Loy */

#include <stdio.h>
#include <carl/carl.h>

/* from crack() */
extern int      arg_index;
extern char    *arg_option;

main(argc, argv)
	int             argc;
	char          **argv;
{
	int             lf, lk, i, j, k, lflag = 0;
	char            crack(), ch;
	float           x, tn, tn1, tn2;
	float          *f, *hk;

	while ((ch = crack(argc, argv, "L|oc", 0)) != NULL) {
		switch (ch) {
		case 'o':
		case 'c':
			break;
		case 'L':
			lf = atoi(arg_option);
			lflag++;
			break;
		}
	}
	if (!lflag)
		lf = atoi(argv[arg_index++]);
	lk = argc - arg_index;
	if (lf <= 0 || lk <= 0)
		exit(1);
	if ((f = (float *) malloc(sizeof(float) * lf)) == NULL)
		exit(1);
	if ((hk = (float *) malloc(sizeof(float) * lk)) == NULL)
		exit(1);

	for (i = 0, j = arg_index; j < argc; i++, j++) {
		hk[i] = expr(argv[j]);
		if (exprerr) {
			fprintf(stderr, "chubby: ill-formed expression: %s\n",
				argv[j]);
			exit(1);
		}
	}

	for (i = 0; i < lf; i++) {
		x = 2.0 * i / (lf - 1) - 1;	/* x is is in a unit interval */
		f[i] = 0;
		tn = 1;
		tn1 = x;
		for (k = 0; k < lk; k++) {
			f[i] = f[i] + hk[k] * tn;	/* add in the weighted
							 * sum */
			tn2 = tn1;	/* preparation for next weighted sum */
			tn1 = tn;
			tn = (2.0 * x * tn1 - tn2);
		}
	}

	if (isatty(1))
		for (i = 0; i < lf; i++)
			printf("%f\n", f[i]);
	else {
		for (i = 0; i < lf; i++)
			putfloat(&f[i]);
		flushfloat();
	}
	exit(0);
}
