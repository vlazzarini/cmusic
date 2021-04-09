#include<stdio.h>
#include<math.h>
#include<carl/frm.h>
#include<carl/carl.h>

#define NHIST 22
int     N = NHIST;
int	norm = 1;
float  *thresh;
int    *bin;

extern int  arg_index;
extern char *arg_option;
extern char crack ();
char    statln[] =
        "min=%6.3f max=%6.3f mode=%6.3f at %6.3f, mean=%6.3f, nsamps=%d\n";

main (argc, argv)
char  **argv;
{
    int     i,
            j,
            modeloc,
            otty = isatty (1), nsamps = 0;
    float   lb = -3.0,
            ub = 3.0,
            input;
    float   mode,
            mean,
            max = -HUGE,
            min = HUGE;
    float   sfexpr ();
    char    ch;

    if (isatty (0))
	histohelp ();

    while ((ch = crack (argc, argv, "l|u|m|n|L|hfN")) != NULL) {
	switch (ch) {
	    case 'l': 
		lb = sfexpr (arg_option, 1.0);
		break;
	    case 'u': 
		ub = sfexpr (arg_option, 1.0);
		break;
	    case 'm': 
		ub = sfexpr (arg_option, 1.0);
		lb = -ub;
		break;
	    case 'n': 
	    case 'L': 
		N = sfexpr (arg_option, 1.0);
		break;
	    case 'f': 
		otty = 1;
		break;
	    case 'N':
		norm = 0;
	    case 'h': 
	    default: 
		histohelp ();
	}
    }
    if (lb >= ub) {
	fprintf (stderr, "stochist: lower bound >= upper bound!\n");
	exit (1);
    }

    thresh = (float *) malloc (sizeof (float) * (N + 1));
    bin = (int *) malloc (sizeof (int) * (N + 1));

    for (i = 0; i <= N; i++)
	thresh[i] = (float) i / N * (ub - lb) + lb;

    while (getfloat (&input) > 0) {
	nsamps++;
	mean += input;
	min = input < min ? input : min;
	max = input > max ? input : max;
	for (i = N; i >= 0; i--)
	    if (input >= thresh[i]) {
		bin[i]++;
		break;
	    }
    }
    if (nsamps == 0)
	exit (1);
    for (mode = 0., i = 0; i < N; i++)
	if (bin[i] > mode) {
	    mode = bin[i];
	    modeloc = i;
	}
    if (mode == 0.) {
	fprintf (stderr, "stochist: no points in histogram window range.\n");
	goto out;
    }

    for (i = 0; i < N; i++) {
	if (otty) {
	    float   x = 63.0 * bin[i] / (norm == 0 ? 1 : mode);
	    printf ("%6.3f %d\t", thresh[i], bin[i]);
	    if (x > 0.0 && x < 1.0)
		printf (".");
	    else
		for (j = 0; j < x; j++)
		    printf ("*");
	    printf ("\n");
	}
	else {
	    float   x = bin[i] / (norm == 0 ? 1 : mode);
	    putfloat (&x);
	}
    }
out: if (!otty)
	flushfloat ();
    else {
	mean /= nsamps;
	printf (statln, min, max, mode, thresh[modeloc], mean, nsamps);
    }
    exit (0);
}

histohelp () 
{
    fprintf (stderr, "%s%s%s%s%s%s%s%s%s%s%s",
	"usage: stochist [flags] < floatsams > floatsams\n",
	"input must be file or pipe of floatsams\n",
	"if output is a terminal, a text histogram is produced\n",
	"if output is file or pipe, histogram sums are written as floatsams\n",
	"flags: (default)\n",
	" -lN	set lower bound of display (-3.0)\n",
	" -uN 	set upper bound of display (3.0)\n",
	" -mN 	set upper and lower bounds of display (3.0, -3.0)\n",
	" -N	suppress normalization of histogram\n",
	" -LN 	set number of histogram windows (22)\n",
	" -f	force output of text bar graph\n"
    );
    exit (1);
}
