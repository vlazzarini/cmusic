/* setfilts.c	1.2	(CARL)	8/4/85	21:33:50 */
#include <sys/types.h>
#include <sndio.h>
#include <vaxuba/dsc.h>
#include <stdio.h>

long
setfilts (srate, ada)
float   srate;
int     ada;
{
	int     flt;
	long    adr = -1L;

 /* find the cutoff point that should be used */
	if (srate > DSCFLT0*2.5)
		flt = 0;
	else
		if (srate > DSCFLT1*2.5)
			flt = 1;
		else		/* remember, DSCFLT2 is bypass */
			if (srate > DSCFLT3 * 2.5)
				flt = 3;
			else	/* below the lowest filter */
				flt = 3;

 /* find the lowest filter that we actually have */
	if (ada == AD)
		switch (flt) {
			case 3: 
				if (DSHASADF1) {
					adr = DS20KHZ;
					break;
				}
			case 1: 
				if (DSHASADF0) {
					adr = DS10KHZ;
					break;
				}
			case 0: 
				if (DSHASADF2) {
					adr = DSBYPAS;
					break;
				}
		}
	else
		switch (flt) {
			case 3: 
				if (DSHASDAF1) {
					adr = DS20KHZ;
					break;
				}
			case 1: 
				if (DSHASDAF0) {
					adr = DS10KHZ;
					break;
				}
			case 0: 
				if (DSHASDAF2) {
					adr = DSBYPAS;
					break;
				}
		}
	if (adr == -1)
		adr = DSBYPAS;	/* dacs and adcs always have bypass */
	return (adr);
}
