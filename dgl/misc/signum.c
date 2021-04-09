#include <stdio.h>
#include <carl/carl.h>

main(argc, argv)
	int argc;
	char **argv;
{
	float output, input, median = 0, scale = 1;
	int otty = isatty(1);

	if (isatty(0))
		usage(1);
	if (argc >= 2)
		median = expr(argv[1]);
	if (argc >= 3)
		scale = expr(argv[2]);

	while (getfloat(&input) > 0) {
		input *= scale;
		if (input < median)
			output = -1;
		else if (input == median)
			output = 0;
		else if (input > median)
			output = 1;
		if (otty)
			printf("%f\n", output);
		else
			putfloat(&output);
	}

	if (!otty) 
		flushfloat();

	exit(0);
}

usage(x)
	int x;
{
fprintf(stderr, "usage: signum [median [gain]] < floatsams > floatsams\n");
exit(x);
}
