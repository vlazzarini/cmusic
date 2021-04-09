#include <stdio.h>

long pad;

getibuf(buf)
	register float *buf;
{
	register int n;

	if ((n = fgetfbuf(buf, BUFSIZ, stdin)) != BUFSIZ) {
		if (n < 0) {
			fprintf(stderr, "lprev: input error\n");
			exit(1);
		}
		while (n < BUFSIZ && pad-- > 0)
			*(buf+n++) = 0.0;
	}

	return(n);
}
