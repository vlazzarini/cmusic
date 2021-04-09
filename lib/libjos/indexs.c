#include "josprogs.h"
#include <ctype.h>

/* INDEXS */

/* indexs: For matching abbreviated keywords to list of full keywords.
 * Return index (<= nstr) in string array stra at which string str appears;
 * -1 if not found, -n if there were n >1 matches. 
 *
 * Uses    struct strarr { char string[CMDSIZ] } stra[]; 
 */

#define	NOTFOUND	-1

int indexs(stra, str, nstr)
register struct strarr stra[];
register char *str;
int nstr;
{
    register int i, lstr; 
    int nhits, ihit;
   
    if (stra == NULL) err("indexs: NULL pointer passed for keyword table");
    if (str == NULL) err("indexs: NULL pointer passed for search key");
    nhits = 0;
    lstr = strlen(str);
    for(i=0; i<nstr; i++)
    {
/* fprintf(stderr,"indexs: comparing /%s/ to /%s/\n",str,stra[i].string); */
	if (strncmp(stra[i].string,str,lstr)==0)
	{
	    nhits += 1;
	    ihit = i;
	}
    }
    if (nhits == 0) return(NOTFOUND);
    if (nhits > 1) return(-nhits);
/*fprintf(stderr,"indexs: Unique hit of %s on %s\n", str, stra[ihit].string); */
    return(ihit);
}
