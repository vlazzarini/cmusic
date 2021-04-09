#include<stdio.h>
#include<math.h>
#include <carl/carl.h>


/*-------------------------------------------------------

			glitch.c

This program calculates a rough approximation to the second
	derivative of the input signal.  It calculates the slope
	at each point in time and compares it to the slope at
	the next point in time.  The N greatest differences
	are saved by the function saveerr.  A glitch should show
	up as a one, two, or three successive large differences with
	alternating signs. 

	cc -O glitch.c -lcarl -lm -o glitch

-------------------------------------------------------*/

main(argc, argv)
	char **argv;
{
	float	in1, in2, oldslope, newslope, err;
	double	agerr = 0;
	long 	nsamps = 2;
	int	N = 22;
	char	ch;

	if (isatty(0))
		usage(1);

/* call crack to interpret command line */

	while ((ch = crack(argc,argv,"N|h",0)) != NULL){
		switch (ch) {
			case 'N': N =  sfexpr(arg_option,1.0); break;
			case 'h': usage(0);
			default:  usage(1);
		}
	}

	if (N > 1024) {
		fprintf(stderr,"glitch: maximum N is 1024\n");
		N = 1024;
	}
	if (N < 1) {
		fprintf(stderr,"glitch: minimum N is 1\n");
		N = 1;
	} 

/* main program */

	if (getfloat(&in1) <= 0)
		exit(1);
	if (getfloat(&in2) <= 0)
		exit(1);
	oldslope = in2 - in1;
	in1 = in2;
	while (getfloat(&in2) > 0) {
		newslope = in2 - in1;
		err = newslope - oldslope;
		saveerr(nsamps, err, N);
		agerr += fabs(err);
		oldslope = newslope;
		in1 = in2;
		nsamps++;
	}
	diag(N);
	printf("average error=\t%f\n", agerr/nsamps);
}

#define HISTLEN 1024
float hist[HISTLEN];
int histind[HISTLEN];
int hi;

/*  hist contains the HISTLEN greatest errors in decreasing order */

saveerr(x, y, N)
	long	x;
	float	y;
	int	N;
{

	hi = N - 1;
	if (fabs(y) <= fabs(hist[hi]))
		return;
	while (fabs(y) > fabs(hist[--hi])) {
		hist[hi+1] = hist[hi];
		histind[hi+1] = histind[hi];
		if (hi == 0){
			hist[0] = y;
			histind[0] = x;
			return;
		}
	}
	hist[++hi] = y;
	histind[hi] = x;
	return;
}

diag(N)
	int N;
{
	register int i, j, k;

	float	list[HISTLEN];
	int	listind[HISTLEN];

	k = 0;

	/* try to group adjacent indices together */

	for (i=0; i < N; i++) if (histind[i] != 0){
		list[k] = hist[i];
		listind[k] = histind[i];
		k++;
		for (j=i+1; j < N; j++){
			if (abs(histind[j] - histind[i]) < 3){
				list[k] = hist[j];
				listind[k] = histind[j];
				histind[j] = 0;
				k++;
			}
		}
	}


	for (i=0; i < N; i++)
		printf("%d:\t%f\n",listind[i],list[i]);
}

usage(exitcode)
	int	exitcode;
{
	fprintf(stderr,"%s%s",
	"\nglitch [-Nnumber] < floatsams\n",
	"\n\t-N  number of possible glitches to report (22)\n");

	exit(exitcode);
}
