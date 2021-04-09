#include <stdio.h>
#include <math.h>
#include <carl/carl.h>
#include <signal.h> 

#define ABS 0
#define NEG 1
#define POS 2

float 	max; 
long 	maxl;
long 	cnt;
int	putout = 0, 
	dBpeak = 0; 

main(narg,argv) 
	int narg; char *argv[]; 
{
	int	die(), i;
	float 	input;
	int 	otty = isatty(1), 
		direction = ABS;
	char 	ch;

	if (isatty(0))
		usage(1);

	while ((ch = crack(narg, argv, "e|h|d|i|a|n|p|", 0)) != NULL) {
		switch (ch) {
			case 'e':	/* -e(rror) */
				putout++;
				break;
			case 'd':	/* -d(B) */
				dBpeak++;
				break;
			case 'i':	/* -i(ndex) */
				break;
			case 'a':	/* -a(bsolute) */
				direction = ABS;
				break;
			case 'n':	/* -n(egative) */
				direction = NEG;
				break;
			case 'p':	/* -p(ositive) */
				direction = POS;
				break;
			case 'h':	/* -h(elp) */
				usage(0);
			default:
				usage(1);
		}
	}

	if (otty && putout) {
		fprintf(stderr, "peak: for -e, output must be file or pipe.\n");
		exit(1);
	}

	for (i = 0; i < NSIG; i++)	/* catch all signals */
	    signal(i, die);		/* set to print peak at time of interrupt */

	switch (direction) {
		case ABS:
		    while ( getfloat(&input) > 0) {
			    if (putout)
				    putfloat(&input);
			    if (input < 0)
				input = -input;
			    if (input > max) {
				    max = input;
				    maxl = cnt;
			    }
			    cnt++;
		    }
		case POS:
		    while ( getfloat(&input) > 0) {
			    if (putout)
				    putfloat(&input);
			    if (input > max) {
				    max = input;
				    maxl = cnt;
			    }
			    cnt++;
		    }
		case NEG:
		    while ( getfloat(&input) > 0) {
			    if (putout)
				    putfloat(&input);
			    if (input < max) {
				    max = input;
				    maxl = cnt;
			    }
			    cnt++;
		    }
	}
	pstat();
	exit(0);
}

usage(x)
	int x;
{
fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s",
"usage:\n",
"peak [flags] < floatsams > peak (ASCII numbers)\n",
"peak [flags] -e < floatsams > floatsams, and >& peak\n",
"\tflags:\n",
"\t-e\tcopy stdin to stdout, print peak on stderr\n",
"\t-d\tprint peak amplitude as dB amplitude\n",
"\t-a\t(default) detect absolute maximum\n",
"\t-n\tdetect negative maximum\n",
"\t-p\tdetect positive maximum\n",
"input must be a file or pipe of floatsams.\n"
);
exit(x);
}

die() 
{
	signal(SIGINT, SIG_IGN);		/* ignore rubouts */
	pstat();
	exit(0);
}

pstat() 
{
	if (putout)
		flushfloat();
	fprintf(putout?stderr:stdout, "%d\t%f\n", 
		maxl,
		dBpeak?(20.0*log10(max>0?max:-max)):(max));
}
