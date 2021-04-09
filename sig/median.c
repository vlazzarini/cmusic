#include <math.h>
#include <stdio.h>
#include <carl/carl.h>
#include <carl/sndio.h>
#include <carl/defaults.h>


/*-------------------------------------------------------

			median.c

This program implements a median filter.  The median filter
	is a nonlinear filter; its output at each point in
	time is the median of the N most recent input values.
	The attraction of such a filter is that - unlike a linear
	filter - it can remove glitches while still following
	step changes perfectly.  On the other hand, the median
	filter does introduce a signal-dependent distortion
	which depends also on the value of N.  

	cc -O median.c -lcarl -lm -o median

-------------------------------------------------------*/


main(argc, argv)
	int argc; char **argv;
{

	int	i,		/* pointer to current sample in buffer */
		j,
		N = 9,		/* number of sample to mediate over */
		N2;		/* N / 2 */

	float	*buf,		/* buffer for N most recent samples */
		min,		/* temporary minimum */
		max,		/* temporary maximum */
		med = 0.,	/* median */
		new,		/* current input sample */
		old,		/* sample being replaced */
		srate = 1.;	/* sample rate */

	char	ch;

	char	*dbuf; 		/* buffer for strings read from header */

	PROP	*proplist;	/* from header on stdin */

/* Read header from stdin. */

	if (isatty(0))
		usage(1);

	if ((proplist =  getheader(stdin)) != NULL) {	/* there is a header */

		if ((dbuf = getprop(stdin,H_SRATE)) != NULL) {
			srate = atof(dbuf);		/* get input srate */
		}
	}

/* Call crack to interpret commandline. */

	while ((ch = crack(argc, argv, "R|", 1)) != NULL){
		switch (ch){
			case 'R': srate = sfexpr(arg_option, 1.0); break;
		}
	}

	arg_index = 0;	/* reset for second call to crack() */

	while ((ch = crack(argc, argv, "R|w|m|h", 0)) != NULL){
		switch (ch){ 
			case 'w': 	N = sfexpr(arg_option, srate); break;
			case 'R': 	break;
			case 'h':	usage(0);
			default: 	usage(1);
		}
	}

	if (N%2 == 0)
		N--;
	N2 = (N-1) / 2;

/* Initialization. */

	if ((buf = (float *) calloc(N,sizeof(float))) == NULL)
		malerr("median: insufficient memory",1);

	for (i = 0; i < N2; i++){
		if (getfloat(&new) <= 0) new = 0;
		buf[i] = new;
		buf[N-i-1] = new;
		if (new > med){
			min = 1e6;
			for (j = 0; j <= i; j++)
				if ((buf[j] >= med) && (buf[j] < min))
					min = buf[j];
			med = min;
		}
		if (new < med){
			max = -1e6;
			for (j = 0; j <= i; j++)
				if ((buf[j] <= med) && (buf[j] > max))
					max = buf[j];
			med = max;
		}
	}

	buf[N2] = med;
	i = N2;

/* Main loop:  buf contains the N most recent inputs.  If input(n) > median
	and input(n-N) > median, then median is unchanged.  The same applies
	if both are less than the median.  Hence the median is only re-
	calculated when input(n) > median and input(n-N) < median or (vice
	versa).  The new median is simply the next largest (or smallest) of
	the N most recent inputs. */

	while (getfloat(&new) > 0){

		old = buf[i];
		buf[i] = new;
		if (++i >= N) i = 0;

		if ((new > med) && (old <= med)){
			min = 1e6;
			for (j = 0; j < N; j++)
				if ((buf[j] >= med) && (buf[j] < min))
					min = buf[j];
			med = min;
		}

		if ((new < med) && (old >= med)){
			max = -1e6;
			for (j = 0; j < N; j++)
				if ((buf[j] <= med) && (buf[j] > max))
					max = buf[j];
			med = max;
		}

		putfloat(&med);

	}

	flushfloat();

}

usage(exitcode)
	int exitcode;
{
	fprintf(stderr, "%s%s%s%s",
	"median [flag] < floatsams > output\n",
	" flags:\n",
	" -wN\t set window size to N seconds (default: 9S).\n",
	" -RN\t set sample rate to N (default = read from stdin).\n"
	);
	exit(exitcode);
}

malerr(str, ex)
	char *str;
	int ex;
{
	fprintf(stderr, "%s\n", str);
	exit(ex);
}

