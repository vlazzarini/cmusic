#include<stdio.h>
#include <carl/carl.h>
#include <carl/defaults.h>

main(argc, argv)
	char **argv;
{
	long m = 0, n = (unsigned) -1 >> 1;
	float input, sampl, srate = FDLOSR;
	int otty = isatty(1);
	char ch;

	while ((ch = crack(argc, argv, "R|", 1)) != NULL) {
		if (ch == 'R') {
			srate = sfexpr(arg_option, 1.0);
			break;
		}
	}

	arg_index = 0;
	while ((ch = crack(argc, argv, "b|d|R|h", 0)) != NULL) {
		switch (ch) {
			case 'R': break;
			case 'b':
				m = sfexpr(arg_option, srate); break;
			case 'd': 
				n = sfexpr(arg_option, srate); break;
			case 'h': usage(0);
			default: usage(1);
		}
	}

	while (m-- && getfloat(&sampl) > 0) /* empty */ ;

	if (otty) {
		while (n-- && getfloat(&sampl) > 0) 
			printf("%f\n", sampl);
	} else {
		while (n-- && getfloat(&sampl) > 0) 
			putfloat(&sampl);
	}

	while (getfloat(&sampl) > 0) /* empty */ ;

	if (!otty)
		flushfloat();
	exit(0);
}

usage(x)
{
fprintf(stderr, "%s%s%s%s%s%s%s%s",
"usage: m4n [-RL] -bM -dN\n < floatsams > text or floatsams",
"flags:\n",
"\t-RN set sampling rate to N (",
DLOSR,
")\n",
"\t-bM set begin time to M\n",
"\t-dN set duration to N\n",
"\tthe usual complement of expressions and postoperators are allowed\n");
exit(x);
}
