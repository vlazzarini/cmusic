#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <carl/carl.h>

int suppress_errors;

main(argc, argv)
	int argc; char **argv;
{
	float output;
	char *parse();
	int nargs = argc;

	while (--nargs) eval(*++argv);
	if (argc == 1 || (argc == 2 && suppress_errors))
		while (eval(parse()));
	}
	
eval(str)
	char *str;
{
	register double output;

	if (str == NULL) return(0);
	if (!strcmp(str, "-e")) {
		suppress_errors++;
		return(0);
	}
	output = expr(str);
	if (exprerr)
		{
		if (suppress_errors)
			printf(" %s ", str);
		else
			printf(" ?%s? ", str);
		return(-1);
		}
	if (floor(output) == output)
		printf(" %.0f ",output); 
	else 
		printf(" %f ",output);
	return(1);
}

char *parse()
{
	static char *c, *b, *lastc;
	static char buf[BUFSIZ];

	c = b+1;

	while (c >= lastc) 
		{
		if (b != NULL) printf("\n");
		if (gets(buf) == NULL) return(NULL);
		for (lastc = buf; *lastc != NULL; lastc++) /* empty */ ;
		c = buf;
		for (c = buf; isspace(*c) && c < lastc; c++) /* empty */ ;
		}
	b = c;
	while (!isspace(*b) && b < lastc) b++;
	*b = NULL;
	return(c);
	}
