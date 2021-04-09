#include<stdio.h>
#include <carl/carl.h>

main(argc, argv)
	char **argv;
{
	int otty = isatty(1);
	float input, output;

	if (argc > 1 || isatty(0))
		usage(1);

	while (getfloat(&input) > 0)
		{
		output = input < 0 ? -input : input;
		if (otty)
			printf("%f\n", output);
		else
			putfloat(&output);
		}

	flushfloat();
	}

usage(x)
{
fprintf(stderr, "usage: rect < floatsams > rectified floatsams\n");
fprintf(stderr, "\tno flags\n");
exit(x);
}
