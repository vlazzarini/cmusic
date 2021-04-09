/* lmaxmin.c	1.1	(CARL)	7/25/84	13:43:13 */
long    lmin (i1, i2)
long    i1,
        i2;
{
	if (i1 < i2)
		return (i1);
	else
		return (i2);
}
