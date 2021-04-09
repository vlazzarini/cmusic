/* malerr.c	2.1	(CARL)	10/26/85	23:43:13 */

/*
 * malerr - complain and exit on malloc() errors
 */

malerr(s, x)
	char *s;
	int x;
{
	printf("%s:malloc", s);
	Pexit(x);
}
