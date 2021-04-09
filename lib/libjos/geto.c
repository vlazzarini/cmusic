#include "josprogs.h"
#include <ctype.h>

/* GETO */

geto(opp,don)
FILE **opp; char *don;
{
    char oname[NAMSIZ];
    oname[0] = NULL;	/* Force getfil to prompt for filenames */
    if (isatty(1)) 		/* Get output */
	{
	    if (don == NULL) don = cat("testout.dat",0);
	    if(!getfil(opp,WRITEM,oname,don)) exit(0);
	    fprintf(stderr," Writing: %s\n", oname);
	    /* Read header here if there is one */
	} 
	else *opp=stdout;
}
