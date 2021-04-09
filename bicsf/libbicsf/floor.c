/* floor.c	1.1	(CARL)	7/25/84	13:42:17 */

/*
 * floor and ceil-- greatest integer <= arg
 * (resp least >=)
 * This is a copy of routine in libm.a so we don't need to drag
 * that whole library around.
 */

double  modf ();

double
        _floor (d)
double  d;
{
	double  fract;

	if (d < 0.0) {
		d = -d;
		fract = modf (d, &d);
		if (fract != 0.0)
			d += 1;
		d = -d;
	}
	else
		(void) modf (d, &d);
	return (d);
}

double
        ceil (d)
double  d;
{
	return (-floor (-d));
}
