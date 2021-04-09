#include "josprogs.h"
#include <ctype.h>

/* GETFIL.C */

/* get a file for input or output */

boolean getfil(fpp, mode, name, dname)
FILE **fpp; char *mode, *name, *dname;
{
    boolean exists;

    if (name == NULL) name = newstr(NAMSIZ);
    if (dname == NULL) dname = newstr(NAMSIZ);
/* We use while instead of if below because for some reason a null filename
   can be opened for reading as if it exists! */
    while ( name == NULL || *name == NULLC ) 	/* ask for a filename */
    {
	if(*mode == *READM) fprintf(stderr," Input file (%s):\n",dname);
	    else fprintf(stderr," Output file (%s):\n",dname);

	name = getsn(name,NAMSIZ);	/* This gets will create name if NULL */
	prsnam(name, dname);		/* fill in name from default name     */
    }

    if (*mode == *APPENDM) 
    {
	*fpp = fopen(name,mode);
	return(TRUE);
    }

    if ( exists = !( (*fpp = fopen(name,READM)) == NULL) )
    {
    if (*mode == *READM) return(TRUE);
	else if (strncmp(name,"/dev/",5) ==0 )
	{
	    *fpp = fopen(name,mode);
	    return(TRUE);
	}
	else
	{
	    if (*mode != *WRITEM) err("getfil: requested mode not r,w, or a");
	    fprintf(stderr,"File %s already exists. Replace?",name);
	    if(sezyes())
	    {
		*fpp = fopen(name,mode);
		return(TRUE);
	    }
		else 
		{ 
		    strcpy(dname,name);
		    *name = NULLC; 
		    return( getfil(fpp,mode,name,dname) ); 
		}
	}
    }
	else	/* file not there */
	if (*mode == *WRITEM)
	{
	    *fpp = fopen(name, mode);
	    return(TRUE);
	}
	    else
	    {
		if (*mode != *READM) err("getfil: file mode not r,w, or a");
		fprintf(stderr,"File %s not found.\n", name);
		strcpy(dname, name);
		*name = NULLC;
		return( getfil(fpp,mode,name,dname) );
	    }
}
