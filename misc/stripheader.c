#include <stdio.h>
#include <carl/carl.h>

main(argc, argv)
	char **argv;
{
	float x;
	int otty = isatty(1);

	noautocp();

	if (otty) {
		while (getfloat(&x) > 0)
				printf("%f\n", x);
	} else {
		while (getfloat(&x) > 0)
				putfloat(&x);
	}
	if (!otty)
		flushfloat();
	exit(0);
}
