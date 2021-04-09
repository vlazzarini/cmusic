/* ptimer.c	2.1	(CARL)	10/26/85	23:43:18 */

ptimer(x)
	char *x;
{
	extern long	Now;

	Now++;
	(void) check_waits();
}
