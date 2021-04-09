
#include <math.h>
#include <stdio.h>
#include <carl/sndio.h>
#include <carl/carl.h>

/*-------------------------------------------------------
			derivative.c

This program takes floatsams on stdin and writes an
	approximation to the derivative on stdout. The
	actual output is 

		output[n] = input[n] - input[n-1]

	which is a good approximation to the derivative
	except at high frequencies.

	cc derivative.c -lcarl -lm

-------------------------------------------------------*/

main(argc, argv)
	int argc;
	char **argv;
{
	float input, output, delay;

	int otty = isatty(1);

	char ch;

	if (isatty(0))
		usage(1);

	while ((ch = crack(argc, argv, "h", 0)) != NULL) {
		switch (ch) {
			case 'h':  usage(0);
			default:   usage(1);	/* this exits with error */
		}
	}

	if (getfloat(&delay) > 0) {
		while (getfloat(&input) > 0) {
			output = input - delay;
			if (otty)
				printf("%6.3f\n", output);
			else
				putfloat(&output);
			delay = input;
		}
		if (!otty)
			flushfloat();
	}
	exit(0);
}

usage(exitcode)
	int exitcode;
{
	fprintf(stderr,"usage: derivative < floatsams > floatsams \n");
	exit(exitcode);
}

