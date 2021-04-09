#include<stdio.h>
#include<math.h>
#include<carl/carl.h>

extern int      arg_index;	/* from crack() */
extern char    *arg_option;
extern double   xrand();

main(argc, argv)
	char          **argv;
{
	extern double   frand();
	int             seq = 1 << 12, cnt;
	long int        ns = 16384;
	int             otty = isatty(1);
	double          div, lb = -1.0, ub = 1.0, rolloff =.05, mixture = 0.0;
	float           output;
	char            ch, weighting = 'w';
	unsigned        seed = 1;
	char           *state;

	while ((ch = crack(argc, argv, "hw|l|u|n|s|r|m|S|", 0)) != NULL) {
		switch (ch) {
		case 'l':
			lb = expr(arg_option);
			break;
		case 'u':
			ub = expr(arg_option);
			break;
		case 'n':
			ns = expr(arg_option);
			break;
		case 's':
			seq = expr(arg_option);
			break;
		case 'r':
			rolloff = expr(arg_option);
			break;
		case 'm':
			mixture = expr(arg_option);
			break;
		case 'S':
			seed = (int) expr(arg_option);
			break;
		case 'w':
			weighting = *arg_option;
			break;
		case 'h':
		case EOF:
		default:
			randomhelp();
		}
	}

	if ((state = malloc(256)) == NULL) {
		fprintf(stderr, "noise: can't malloc random state\n");
		exit(1);
	}
	initstate(seed, state, 256);

	switch (weighting) {
	case 'w':		/* plain old vanilla white noise */
		for (cnt = 0; cnt < ns; cnt++) {
			output = frand(lb, ub);
			if (otty)
				printf("%f\n", output);
			else
				putfloat(&output);
		}
		break;
	case 'i':		/* integer white */
		for (cnt = 0; cnt < ns; cnt++) {
			register int    ilb = lb, iub = ub;
			output = irand(ilb, iub);
			if (otty)
				printf("%f\n", output);
			else
				putfloat(&output);
		}
		break;
	case 'f':		/* 1/f noise */
		for (cnt = 0; cnt < ns; cnt++) {
			output = onefrand(lb, ub, seq);
			if (otty)
				printf("%f\n", output);
			else
				putfloat(&output);
		}
		break;
	case 'm':		/* mix of white, 1/f and correlated */
		for (cnt = 0; cnt < ns; cnt++) {
			output = xrand(lb, ub, rolloff, mixture, seq);
			if (otty)
				printf("%f\n", output);
			else
				putfloat(&output);
		}
		break;
	case 'b':		/* correlated noise */
		for (cnt = 0; cnt < ns; cnt++) {
			output = brand(lb, ub, rolloff);
			if (otty)
				printf("%f\n", output);
			else
				putfloat(&output);
		}
		break;
	default:
		fprintf(stderr, "noise: no such weighting: %c\n",
			weighting);
		randomhelp();

	}
	flushfloat();
}

randomhelp()
{
	fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"usage: noise [flags]\n",
		" flags:\n",
		" -l = lower bound (-1.0)\n",
		" -u = upper bound (+1.0)\n",
		" -n = number of samples(16384)\n",
		" -S = seed (0)\n",
		" -w = weighting (w),\n",
	      "      options are: w = white, f = 1/f, i = rational white\n",
		"      b = brownian, m = mixed\n",
		" -s = sequence length(4096) for 1/f\n",
		"      low sequence lengths tend towards whiter noise\n",
		" -r = relative distance traveled for brownian (.1)\n",
		"      range: 0 (correlated) => 1 (uncorrelated)\n",
		" -m = mixture of noise type for -wm mode:\n",
		"      -1 => 0 mix b and f, 0 => +1 mix f and w\n"
		);
	exit(1);
}
