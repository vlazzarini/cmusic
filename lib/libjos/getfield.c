#include "josprogs.h"
#include <ctype.h>

/* GETFIELD */

getfield(infile, string, tklist,lstr)
FILE *infile; char *string, *tklist; int lstr; 
{   int nstr; 
    boolean leading, comment; 
    char c;
    leading = TRUE;	/* Begin as if in white space between tokens */
    comment = FALSE;
    nstr=1;		/* Size of token so far */
    *string = NULL;
    while( (c=getc(infile)) != EOF){
	if(nstr>=lstr)
	{
	    *string = NULL; 
	    fprintf(stderr,"getfield: token '%s' too long\n",string); 
	    return(c);
	}
	if('A' <= c && c <= 'Z') c += 'a' - 'A';
	if(c=='#')comment = TRUE;
	if(comment && c=='\n') comment = FALSE;
	if(comment)continue;
	if(leading && (c==' ' || c=='\n' || c=='\t'))continue;
/* 	printf("index(%s,%c)=%d\n",tklist,c,index(tklist,c));  	*/
	if (index(tklist,c)) {*string++ = c; nstr++; leading = FALSE; continue;}
	    else if ( !leading )
	    {
		*string = NULLC; 
	        /* printf("getfield: returning /%s/\n",(string-nstr+1)); */
		return(c);
	    }
    }
    return(EOF);
}
