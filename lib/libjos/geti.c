#include "josprogs.h"
#include <ctype.h>

/* GETI */

geti(ipp,din)
FILE **ipp; char *din;
{
    char iname[NAMSIZ];
    iname[0] = NULL;	/* Force getfil to prompt for filenames */
    if (isatty(0))			/* Get input */ 
	{
	    if (din == NULL) din = cat("testin.dat",0);
	    if (!getfil(ipp,READM,iname,din))exit(0);
	    fprintf(stderr," Reading: %s\n", iname);
	    /* Read header here if there is one */
	} 
	else *ipp=stdin;
}
