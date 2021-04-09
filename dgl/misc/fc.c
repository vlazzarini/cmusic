#include <stdio.h>
#include <carl/carl.h>
#include <signal.h>

/* fc - floatsam-count, returns count of floatsams from standard input */

long    cnt;

main () 
{
	extern int icatch();
	float   input;

	if (isatty (0)) {
		fprintf (stderr, "usage: fc < floatsams > count\n");
		fprintf (stderr, "Input must be file or pipe.\n");
		exit (1);
	}
	signal(SIGINT, icatch); 
	while (getfloat (&input) > 0)
		cnt++;
	printf ("%d\n", cnt);
}

icatch()
{
	printf ("%d\n", cnt);
	exit(0);
}
