#include <stdio.h>
#include <carl/carl.h>

int otty;
char *chanspec;
int nc;

extern char *chans;

main(argc, argv)
	char **argv;
{
	char ch;
	float sample;
	register int nc = 1;
	register long cnt = 0;

	otty = isatty(1);

	while ((ch = crack(argc, argv, "C|c|ht", 0)) != NULL) {
		switch (ch) {
			case 'c': chanspec = arg_option; break;
			case 'C': nc = sfexpr(arg_option, 1.0); break;
			case 't': otty = 1; break;	/* force tty output */
			case 'h': usage(0);
			default: usage(1);
		}
	}
	if (setchan(chanspec, nc)) 
		usage(2);

	if (!otty) {
		register int y = -1;

		while(getfloat(&sample) > 0) {
			if (!chans[++y >= nc ? (y = 0)  : y]) 
				continue;
			putfloat(&sample);
		}
	} else {
		register int y = -1;
		register long i = -1;
		int x;

		while(getfloat(&sample) > 0) {
			i++;
			if (!chans[++y >= nc ? (y = 0)  : y]) 
				continue;
			printf("%d\t%f\n", i, sample);
		}
	}

	if (!otty) flushfloat();

	exit(0);
}

usage(x)
{
	fprintf(stderr, "%s%s%s%s%s",
	    "usage: channel [flags] < floatsams > floatsams or text\n",
	    "\tflags:\n",
	    "\t-CS\tS total number of channels\n",
	    "\t-cS\tS is a comma-separated list of selected channel numbers\n",
	    "\t-t\tforce text output instead of floatsams\n"
	    );
	exit(x);
}
