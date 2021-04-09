/* step - to compile this beastie utter: cc -O step.c -lsf -lfrm -lm */

#include <stdio.h>
#include <carl/carl.h>
#include <math.h>

/* from crack() */
extern int arg_index;
extern char *arg_option;

main(argc, argv)
	int argc; char **argv;
{
	int flen, lk, i, j, k, lflag=0;
	char crack(), ch;
	float maxx = 0, minx = HUGE, fi, fj;
	float *f, *xval, *yval;

	if (argc < 3)
		exit(1);
	while ((ch = crack(argc, argv, "L|oc", 0)) != NULL) {
		switch(ch) {
			case 'o':
			case 'c':	
				break;
			case 'L':	
				flen = atoi(arg_option); 
				lflag++; 
				break;
		}
	}
	if (!lflag) 
		flen = atoi(argv[arg_index++]);
	lk = (argc - arg_index)/2;
	if (lk <= 0)
		exit(1);
	if ((xval = (float *) malloc(sizeof(float)*lk)) == NULL)
		exit(1);
	if ((yval = (float *) malloc(sizeof(float)*lk)) == NULL)
		exit(1);

	for (i = 0, j = arg_index, k = 0; j < argc; i++, j++) {
		if (i % 2)
			yval[k++] = expr(argv[j]);
		else {
			xval[k] = expr(argv[j]);
			maxx = xval[k] > maxx ? xval[k] : maxx;
			minx = xval[k] < minx ? xval[k] : minx;
		}
		if (exprerr) {
			fprintf(stderr, "step: ill-formed expression: %s\n",
				argv[j]);
			exit(1);
		}
	}

	if (isatty(1)) {
		for (i = j = 0; i < flen; i++) {
			if (i >= xval[j+1] && j+1 < lk) 
				j++;
			printf("%f\n", yval[j]);
		}
	}
	else {
		for (i = j = 0; i < flen; i++) {
			if (i >= xval[j+1] && j+1 < lk) 
				j++;
			putfloat(&yval[j]);
		}
		flushfloat();
	}
	exit(0);
}
