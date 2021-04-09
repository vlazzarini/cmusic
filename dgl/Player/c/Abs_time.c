/* Abs_time.c	2.2	(CARL)	1/30/86	16:31:26 */

# include <carl/Player.h>

P_TYPE Abs_time()
/*
 * Return absolute current time in global variable \fBNow\fP.
 * It's quicker just to get the value of \fBNow\fP directly.
 * This routine is useful where one wants to guarantee that
 * \fBNow\fP is only being referenced by user code.
 * \fBP_TYPE\fP is defined as 'double' on machines with
 * floating point, otherwise it is 'long'.
 */
{
	return(Now);
}
