/* clexer.c	2.1	(CARL)	10/26/85	23:49:27 */

#include <stdio.h>
#include "trans.h"
#include "y.tab.h"
#include "clexer.h"
#include "stats.h"
#include "ascii.h"

extern int bug;

#define dstat 	1
#define dlex	2

int linecnt = 1;	/* used globally to count input lines */

int curstate;
char str[1024*8];
char *tokenstr;

clexer()
{
	extern YYSTYPE yylval;
	char c, class, *transchr, nullc = '\0';
	int i;
	struct trans *curtrnsptr;

	tokenstr = str;
	class = ascii[(c = getchar())]; /* get character classification */
	if (c == '\n')
		linecnt++;
	*tokenstr++ = c;
	curstate = 1;
	curtrnsptr = statab[curstate].trnsptr;
	if ((transchr = curtrnsptr->transchr) == NULL)
		transchr = &nullc;
	while ((*transchr != '\0') && (c != EOF) && (class != '\0')) {
		while (*transchr != '\0') {
			if (*transchr == class) {
				if (bug) printf("%d:%c ",curstate,c);
				curstate = curtrnsptr->nxtstate;
				class = ascii[(c = getchar())]; /* get next */
				*tokenstr++ = c;
				curtrnsptr = statab[curstate].trnsptr;
				if ((transchr = curtrnsptr->transchr) == NULL)
					transchr = &nullc;
			}
			else {
				*transchr++;
			}
		}
		*curtrnsptr++;
		if ((transchr = curtrnsptr->transchr) == NULL)
			transchr = &nullc;
	}
	if (c != EOF) {
		*--tokenstr = '\0'; /* clobber last chr on string */
		if (class != '\0') ungetc(c,stdin); /* put back for next time */
		if ((curstate != 9) && (bug)) 
			printf("%s\t\t%s\n",str, statab[curstate].lexem);
		yylval.s = str;
		return(statab[curstate].lexno);
	}
	else {
		*--tokenstr = '\0';
		yylval.s = str;
		return(EOF);
	}
}
