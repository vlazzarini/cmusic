#include <stdio.h>
#include <carl/carl.h>
#include <carl/defaults.h>

/*
 * readheader - read a header from stdin, format and print it on stdout,
 * optionally print any samples after the header.
 * If no header, data read will be interpreted as floats.
 */

main(argc, argv)
	char **argv;
{
	PROP *proplist;
	short perr;
	short s, *ps = &s;
	float x;
	char ch;
	register int otty = isatty(1), cnt = 0;

	while ((ch = crack(argc, argv, "e", 0)) != NULL) {
		if (ch == 'e')
			perr++;
	}

	if ((proplist = getheader(stdin)) != NULL) {
		printprop(proplist, perr?stderr:stdout);
	} else {
		fprintf(stderr, "readheader: no header\n");
	}

	if (perr) {
		while (getfloat(&x) > 0) {
			if (otty)
				printf("%d\t%f\n", cnt++, x);
			else
				putfloat(&x);
		}
		flushfloat();
	}
	exit(0);
}
