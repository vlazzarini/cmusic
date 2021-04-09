#include<stdio.h>
#include <math.h>
#include <ctype.h>
#include <carl/carl.h>
#include "list.h"
extern int arg_index;		/* from crack() in -lsf */
extern char *arg_option;

float Pi, Piovr2, Piovr4;
int tanit = 1, verbose = 0;
char outop;

main(argc, argv)
	char **argv;
{
	LIST *rfun(), *Head, *L;
	FILE *fp, *fopen();
	char ch, crack();
	float sfexpr(), getdistance(), *buf, *obuf, Offset;
	int otty = isatty(1), n, i, nchans = 4, 
		chan = -1, first = 1, len, pos=0, olen=1024;
	float dur, inc, theta, sig1, sig2, distance, gamp, grev, dopp;
	float  getheta(), getcoeff(), *interp(), doppler();
	int getquad();
	float vsound = 340.0;	/* ~ velocity of sound */
	Pi = 4.0 * atan(1.0);
	Piovr2 = Pi/2.0;
	Offset = Piovr4 = Pi/4.0;
	if (argc < 2) exit(-1);

	while ((ch = crack(argc, argv, "o|L|d|v|1|2|3|4|artbh", 0)) != NULL) 
	    {
	    if (isdigit(ch)) { chan = ch - '1'; outop = 'c'; }
	    else
	    switch (ch)
		{
		case 'd': dur = sfexpr(arg_option, 1.0); outop = ch; break;
		case 'v': vsound = sfexpr(arg_option, 1.0); break;
		case 't': tanit=0; break;
		case 'a': /* goes with r */
		case 'r': outop = ch; break; 
		case 'L': olen = sfexpr(arg_option, 1.0); break;
		case 'o': Offset = sfexpr(arg_option, 1.0);
			Offset = 2.0 * Pi * Offset / 360.0; break;
		case 'b': verbose++; break;
		case 'h': amphelp(); exit(0);
		default:  amphelp(); exit(-1);
		}
	    }

	if (verbose)
		{
		for (i = 0; i < argc; i++) fprintf(stderr, "%s ", argv[i]);
		fprintf(stderr, "\n");
		}

	if (arg_index >= argc)
		{
		fprintf(stderr, "quad: missing filename\n");
		exit(-1);
		}

	if ((Head = rfun(Head, argv[arg_index], 0.0, 0.0, 1.0, 1)) == NULL) 
		{
		fprintf(stderr, "quad: error reading %s\n", argv[arg_index]);
		exit(-1);
		}

	/* 
	 * normal way of specifying doppler duration:
	 * quad -dN file
	 * alternate way of specifying doppler duration:
	 * quad -d file N
	 * This format allows some convenience when called from cmusic
	 */

	if (outop == 'd' && dur == 0.0 && arg_index == argc-2)
		dur = sfexpr(argv[arg_index+1], 1.0); 

	if (outop == 'd' && dur <= 0.0)
		{
		fprintf(stderr,"quad: error in specifying doppler duration\n");
		exit(-1);
		}

	if (outop == 'd')
		{
		for (L = Head, n = 0; L; L = L->next) n++; /* count'em up */
		inc = n/dur;
		}

	for (i = 0, L = Head; L; L = L->next)
		{
		float scale, scale1, coeff; 
		float ch[4];
		int q;
		theta = getheta(L->fx, L->fy);
		q = getquad(theta, Offset);
		coeff = getcoeff(theta, Offset);
		getscale(coeff, &scale, &scale1);
		ch[0] = ch[1] = ch[2] = ch[3] = 0.0;
		switch (q) {
			case 1: ch[0] = scale; ch[1] = scale1; break;
			case 2: ch[1] = scale; ch[2] = scale1; break;
			case 3: ch[2] = scale; ch[3] = scale1; break;
			case 4: ch[3] = scale; ch[0] = scale1; break;
			}
		distance = getdistance(L->fx, L->fy);
		if (distance < 1.0) distance = 1.0;
		gamp = 1.0/distance;
		grev = 1.0/sqrt(distance);
		dopp = doppler(L->fx, L->fy, vsound, inc);
		
		switch (outop) {
			case 'c': len = fsave(ch[chan], &buf, pos++, len);break;
			case 'a': len = fsave(gamp, &buf, pos++, len); break;
			case 'r': len = fsave(grev, &buf, pos++, len); break;
			case 'd': 
				if (first) {first=0; break; }
				len = fsave(dopp, &buf, pos++, len); break;
			}
		}
	obuf = interp(buf, pos-1, olen);
	if (otty)
		for (i = 0; i < olen; i++) printf("%f\n", obuf[i]);
	else
		{
		for (i = 0; i < olen; i++) putfloat(&obuf[i]);
		flushfloat();
		}
	exit(0);
	}
/*
 * /1* now in nodfun.c *1/
 * float getdistance(x, y)
 * 	float x, y;
 * {
 * 	return(sqrt(x*x+y*y));
 * 	}
 */

/*
 * /1* now in nodfun.c *1/
 * float getheta(x, y)
 * 	float x, y;
 * {
 * 	register float theta;
 * 	theta = atan2(y, x);
 * 	if (theta < 0.0) theta = (Pi + theta) + Pi;
 * 	return(theta);
 * 	}
 */

/*
 * /1* now in nodfun.c *1/
 * float circlemod(x) 
 * 	float x;
 * {
 * 	register float twopi = 2.0 * Pi;
 * 	while (x > twopi) x -= twopi;
 * 	while (x < 0) x += twopi;
 * 	return(x);
 * 	}
 */



float getcoeff(theta, offset)
	float theta, offset;
{
	float rtn;
	int q;
	theta += offset;
	while (theta >= Piovr2) theta -= Piovr2;
	return(theta);
	}

getscale(coeff, scale1, scale2)
	float coeff, *scale1, *scale2;
{
	register float tmp;
	if (tanit)
		{
		*scale1 = sqrt(1.0 - .5 * (1.0 + tan(coeff - Piovr2/2.0)));
		*scale2 = sqrt(.5 * (1.0 + tan(coeff - Piovr2/2.0)));
		}
	else
		{
		*scale1 = sqrt(1.0 - coeff/Piovr2);
		*scale2 = sqrt(coeff/Piovr2);
		}
	}

float doppler(x, y, vsound, inc)
	float x, y, vsound, inc;
{
	static float dlast;
	register float dcur, dD, u, ds;
	dcur = sqrt(x*x + y*y);
	dD = dcur - dlast;
	u = dD * inc;
	ds = vsound/(vsound + u);
	dlast = dcur;
	return(ds);
	}

fsave(x, buf, i, len)
	float x, **buf; int i, len;
{
	float *b;
	if (*buf == NULL) {
		*buf = (float *) calloc(sizeof(float)*BUFSIZ, 1);
		len = BUFSIZ;
		}
	if (i < len)
		(*buf)[i] = x;
	else {
		len += BUFSIZ;
		*buf = (float *) realloc(*buf, len * sizeof(float));
		(*buf)[i] = x;
		}
	return(len);
	}


/*
 * take buf with nipts floats, and return a linearly interpolated array
 * nopts long.
 */
getquad(theta, offset)
	float theta, offset;
{
	theta = circlemod(theta-offset);
	if (theta < Piovr2) return(1);
	else
	if (theta < Pi) return(2);
	else
	if (theta < 3.0*Piovr2) return(3);
	else
	return(4);
	}

float *interp(buf, nipts, nopts)
	float *buf; int nipts, nopts;
{
	register int i, c;
	register float rat, fc, frat, *ntbuf;
	rat = (float) nipts / (float) nopts;
	ntbuf = (float *) malloc(sizeof(float)*nopts);
	for (i = fc = c = 0; i < nopts; fc += rat, i++) 
		{
		c = fc;		/* truncate */
		frat = fc - c;	/* get fraction */
		ntbuf[i] = (1.0-frat)*buf[c] + frat*buf[c+1];
		}
	return(ntbuf);
	}
amphelp()
{
fprintf(stderr, "%s%s%s%s%s%s%s%s%s",
"usage: quad [flags] space_path\n",
"quad reads functions of time as text [x,y] pairs (1 pair per line)\n",
"and calculates Chowning-style amplitude functions for quad distribution.\n",
"flags:\n",
" -LN cause functions to be scaled to length N\n",
" -dN set duration of entire function to N seconds\n",
" -N  (where N = one of {1,2,3,4}) calculate and output that channel\n",
" -a  calculate and output global amplitude\n",
" -r  calculate and output global reverb.\n"
);
}
