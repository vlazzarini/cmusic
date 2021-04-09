#include "josprogs.h"
#include <ctype.h>

/* PRSNAM */

/* Return filename consisting of that which   
 * is specified in name plus any extra fields 
 * present in the default name dname. 	      
 * A full name is head/body.tail or 	      
 * directory-path/name.extension if you will. 
 * There are a few peculiarities in overriding defaults. 
 * To specify a filename with no tail, one must include
 * a `.' after the name which signifies a null tail. To
 * get a filename followed by a single `.', type two dots
 * and for two, type three, etc. 
 * Finally, to get rid of a default directory, type ./ followed
 * by carriage return.
 */

prsnam(name, dname)	
char *name, *dname;		
{
    char *nhead, *nbody, *ntail, *dhead, *dbody, *dtail;
    char *ohead, *obody, *otail; 
    char *oname, *t;

 /* fprintf(stderr,"prsnam gets name = /%s/, dname = /%s/\n",name,dname); */
    if (*dname == NULLC) return;
    if (*name == NULLC) { strcpy(name,dname); return; }

    nhead = gethead(name);
    dhead = gethead(dname);
    nbody = getbody(name);
    dbody = getbody(dname);
    ntail = gettail(name);
    dtail = gettail(dname);

# define E(name) (strlen(name)!=0)		/* `Exists' */

    if(E(dhead) BUT !E(nhead)) ohead = cat(dhead,0); else ohead = cat("",nhead);
    if(E(dbody) BUT !E(nbody)) obody = cat(dbody,0); else obody = cat("",nbody);
    if(E(dtail) BUT !E(ntail)) otail = cat(dtail,0); else otail = cat("",ntail); 
/* Special hack for trailing `.' */
    for(t=otail; *t == '.'; t++);
    if(*otail != NULLC && (t-otail) == strlen(otail) ) otail++;

/* printf("prsnam: head = /%s/, body = /%s/, tail = /%s/\n",ohead,obody,otail); */
    oname = catm(ohead, obody, otail, NULL);
    strcpy(name, oname);
    if(strlen(name) == 0)
    fprintf(stderr," prsnam: Warning: Your filename is null which means `.'\n");
 /* fprintf(stderr,"prsnam returns with name = /%s/, dname = /%s/\n",name,dname); */
}
