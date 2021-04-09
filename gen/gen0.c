/* gen0 - scale signal to be bounded by [-scale,+scale] */

#include<stdio.h>
#include<carl/carl.h>

main(argc, argv)
	char          **argv;
{
	float           input, output, scale = 1.0, *func, max = 0;
	int             otty = isatty(1);
	char            ch;
	long            len = (unsigned) -1 >> 1, cnt = 0;

	while ((ch = crack(argc, argv, "L|", 0)) != NULL) {
		switch (ch) {
		case 'L':
			len = expr(arg_option);
			if (exprerr) {
				fprintf(stderr, "expression error: %s\n",
					arg_option);
				exit(1);
			}
			break;
		default:
			fprintf(stderr,
			   "usage: gen0 -LN max < floatsams > floatsams\n");
			exit(1);
		}
	}

	if (arg_index < argc) {
		scale = expr(argv[arg_index]);
		if (exprerr) {
			fprintf(stderr, "gain expression error: %s\n",
				argv[arg_index]);
			exit(1);
		}
	}
	while ((getfloat(&input) > 0) && cnt < len) {
		if (input < 0)
			input = -input;
		if (input > max)
			max = input;
		if (floatsav(&func, input) < 0)
			exit(1);
		cnt++;
	}

	max = scale / max;
	for (len = cnt, cnt = 0; cnt < len; cnt++) {
		output = func[cnt] * max;
		if (otty)
			printf("%6.3f\n", output);
		else
			putfloat(&output);
	}
	if (!otty)
		flushfloat();
	exit(0);
}
