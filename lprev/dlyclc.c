#include <stdio.h>

main(argc, argv)
	int argc; char **argv;
{
	float f, srate, atof();
	int i, sum;
	argc--;
	srate = atof(*++argv);
	printf("srate = %f\n", srate);
	while (--argc)
		{
		f = atof(*++argv) * srate;
		i = f;
		printf("dly time= %s dly len= %d\n", *argv, i);
		sum += i;
		}
	printf("sum = %d\n", sum);
	}
