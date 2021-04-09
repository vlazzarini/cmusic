#include <string.h>
#include <stdio.h>
#include "mm.head.h"

getfield(infile, string, iglist,  brklist, listfile)
 FILE *infile; char *string, *iglist, *brklist; FILE *listfile;
{
    int leading, c, cc; static int clevel = 0, qlevel = 0;
    static char lastc, realc = '\n';
    leading = 1;
    *string = NULL;
    while( (lastc=realc, realc=c=getc(infile)) != EOF){
/* produce listing file or verbose output if needed (neatify, if needed) */
	if(listfile){ 
	    if(lastc == ';' && c != '\n')fputc('\n',listfile);
	    if(c != '\t')fputc(c,listfile); else fputc(' ',listfile);
	    fflush(listfile);
	}
	if(Verbose){ 
	    if(lastc == ';' && c != '\n')fputc('\n',stderr);
	    fputc(c,stderr); 
	}
/* ignore lines which START with # */
	if(c == '#') {
	    if(lastc=='\n')
		while( (realc = c = getc(infile)) != '\n');
	    continue ;
	}

/* process quoted strings literally */
	if(!clevel && c == '"'){qlevel ^= 1; continue;}
	if(!clevel && qlevel){*string++ = c; leading = 0; continue;}
/* turn tabs and newlines into blanks */
	if(c == '\t')c = ' ';
	if(c == '\n')c = ' ';
/* ignore (possibly nested) comments between curly braces */
	if(c == '{'){clevel++; continue;}
	if(c == '}'){clevel--; continue;}
	if(clevel < 0){PROUT("**Unbalanced {}'s**\n",c); clevel = 0;}
	if(clevel > 0)continue;
/* ignore leading blanks */
	if(leading && c==' ')continue;
/* ignore characters on the ignore list */
	if(index(iglist,c))continue;
/* check for break characters */
	if(!index(brklist,c)){
	    *string++ = c; leading = 0; continue;
	} else {
	    cc = *string = NULL; 
/* watch out for blanks, tabs, newlines before break characters */
	    while(c==' ' || c=='\t' || c=='\n'){
		cc = c; 
		c = getc(infile);
	    }
	    if(!index(brklist,c)){ungetc(c,infile); c = cc;}
	     else realc = c;
	    if(cc != NULL){
		if(listfile){ 
		    if(c != '\t')fputc(c,listfile); else fputc(' ',listfile);
		    fflush(listfile);
		}
		if(Verbose)fputc(c,stderr);
	    }
	    return(c);
	}
    }
    if(clevel)PROUT("**Unbalanced {}'s at EOF**\n",c);
    if(qlevel)PROUT("**Unbalanced quotes at EOF**\n",c);
    return(EOF);
}
