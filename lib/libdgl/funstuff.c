#include<stdio.h>
#include <math.h>
#include <carl/fun.h>
#include <carl/carl.h>
#include <strings.h>
#include <stdlib.h>


/*
 * main(argc, argv)
 * 	char **argv;
 * {
 * 	int i;
 * 	float x;
 * 	int nsamps = 23;
 * 	FUN *f, *rautoxfun();
 * 	f = rautoxfun(argv[1]);
 * /1*	for (i = f->min_x; i < f->max_x; i++)*1/
 * /1*		printf("%f\n", f->y[i]);*1/
 * /1*	for (i = 0, x = 0; x < f->max_x; x += f->max_x/10)*1/
 * 	for (i = 0; i < nsamps; i++)
 * 		{
 * 		float y, ntrpfun();
 * 		x = (float) i/nsamps;
 * 		x *= f->max_x;
 * 		y = ntrpfun(f, x);
 * 		printf("%d:\t%3.1f:\t%f\n", i++, x, y);
 * 		}
 * 	}
 */

/*
 * rautoxfun - read function with automatic abcissas - only y values supplied,
 * x assumed to go from 0 to EOF
 * file must be binary floating point numbers
 */

FUN *rautoxfun(name)
	char *name;
{
	FUN *f;
	FILE *fp, *fopen();
	int bufsiz = BUFSIZ, sizeofbuf = sizeof(float)*BUFSIZ;
	register int i = 0;
	float input;

	if ((fp = fopen(name, "r")) == NULL) return(NULL);

	f = (FUN *) calloc(sizeof(FUN), 1);
	f->y = (float *) malloc(sizeof(float)*BUFSIZ);
	f->min_y = HUGE; f->max_y = -HUGE;

	while (fgetfloat(&input, fp) > 0)
		{
		if (i >= bufsiz) f->y = 
			(float *) realloc(f->y, bufsiz+=sizeofbuf);
		*(f->y+i++) = input;
		if (input < f->min_y) f->min_y = input;
		if (input > f->max_y) f->max_y = input;
		}
	fclose(fp);
	f->min_x = 0; f->max_x = i-1;
	f->y = (float *) realloc(f->y, i*sizeof(float));
	return(f);
	}


/*
 * ntrpfun - interpolate function f by x.  No checking that x lies within
 * range of f.
 */

float ntrpfun(f, x)
	FUN *f; float x;
{
	int is1, is2;
	float s1, s2;
	int ix1 = x, ix2;
	if (ix1 < f->max_x ) ix2 = ix1+1;	/* might have to wrap around */
	else ix2 = 0;
	return(f->y[ix1] + ((x - ix1) * (f->y[ix2] - f->y[ix1])));
	}


/*
 * incrfun - treat function f as though made up of n points, indexed by i
 */

float incrfun(f, i, n)
	FUN *f; int i, n;
{
	register float x, y; 
	float ntrpfun();
	x = (float) i/n;
	x *= f->max_x;
	return(ntrpfun(f, x));
	}
