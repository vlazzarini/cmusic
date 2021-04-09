/* globals.c	1.3	(CARL)	9/12/86	16:29:57 */

/*
 * Note: with the advent of 4.3BSD, the size of the file descriptor table
 * became variable, and should be determined dynamically by getdtablesize(2).
 * However, rather than perform the moderate upgrade this would require
 * to this code, I have opted for the cheap way out, and derive this
 * directly from /sys/h/param.h.  Anyone ambitious enough to fix this to
 * do it right is welcome to it.	=dgl
 */

#include <sys/param.h>
#include <carl/procom.h>

#define _NOFILE 64			/* a kludge of sorts */
struct fltbuf fb[_NOFILE];
short _samplesize = sizeof(float);
