/* polish.c	1.2	(CARL)	8/27/84	12:42:40 */
/*
 * polish() is a copy of the routine from libfrm.a, put here so we don't
 * have to require that library all the time.
 */

#include <stdio.h>
#include <ctype.h>
extern char *malloc(), *calloc(), *realloc();
#define ENDLIST(p,n,r) for(p=r; p->n; p=p->n)
#define FORLIST(p,n,r) for(p=r; p; p=p->n)
#define LINK(p,n,r,t) if(!r)r = p = NEWNODE(t);\
else { \
	ENDLIST (p, n, r); \
	\
		if ((\
				p -> n = NEWNODE (t) \
			) == NULL) { \
		fprintf (stderr, "\nPOLISH: CALLOC FAILED!\n"); \
		exit (1); \
	} \
		p = p -> n; \
}
#define NEWNODE(type) (struct type *) calloc(1, (unsigned) sizeof(struct type))
#define OPMAX 20
#define SMAX 50
#define UNOP 1
#define BINOP 2
#define POSTOP 4
#define OPND 8
#define LPN 16
#define INIT 32
#define RPN 64

/* SDY 
extern char   *strcat ();
extern char   *strcpy (), *strncpy (), *index (), *rindex ();
extern char   *sprintf ();
*/

/* 
  char *POLISH (EXPRESSION, UNOPS, BINOPS, POSTOPS);
   char *EXPRESSION, *UNOPS, *BINOPS, *POSTOPS;

   POLISH processes 4 strings as arguments: EXPRESSION is a string containing 
   an expression to be transformed into reverse polish notation, and UNOPS, 
   BINOPS, and POSTOPS are strings describing unary, binary, and post operators 
   to be recognized in the expression. Anything in the expression not 
   recognized as an operator is taken to be an operand.  A pointer to a 
   (static) string containing the polish form of EXPRESSION is returned.  
   The operator strings are ordered lists of sets, with operators in the first
   set done before operators in the second set, which are done before operators
   in the third, etc. Sets are notated as a list of comma-separated items
   enclosed in braces ("{}"). For example, the string "{*,/}{+,-}" as BINOPS 
   would specify that "*" and "/" are to be done before "+" and "-".  Unary 
   operators are always done before binary, and binary before post operators. 
   Operators may be more than single characters; in particular, functions
   and post operators may be treated as multicharacter UNOPS and POSTOPS.
   POLISH returns a string in reverse polish notation consisting of 
   comma-separated fields, each containing a symbolic item (either an operator 
   or an operand) followed by a dollar sign, followed by a digit. The digit 
   is the number of operands of the item, which is zero for operands, 1 for 
   unary and post operators, and 2 for binary operators. (Someday function 
   calls, which are treated as UNOPS, may allow more than 1 argument as well).
   Example:
    polish("-3+(-p4-ln(v3)*p5)Hz","{sin,cos,ln}{-}","{*,/}{+,-}","{dB,Hz}");
     will return the following string:
    "3$0,-$1,p4$0,-$1,v3$0,ln$1,p5$0,*$2,-$2,+$2,Hz$1,"
*/
char   *polish (expression, unops, binops, postops)
char   *expression,
       *unops,
       *binops,
       *postops;
{
	extern char *index(), *strcpy();
	char   *expr = expression,
	       *ptr[3];
	int     lmatch,
	        p = 0,
	        prev = INIT,
	        top = 0;
	static struct operator {
		char    op[OPMAX];
		int     prec;
		int     otype;
		int     nopnds;
		struct operator *next_o;
	}                      *o, *oo, S[SMAX], *olist = NULL;
	char    fld[100],
	        bc;
/* SDY
	extern char    *sprintf ();
*/
	static int      firsttime = 1;
	static char     os[1000];
	int     i;
/*
 * Create linked operator list on first call only.  To be able to revise
 * operator lists on each call, remove the firsttime test and the comments
 * around the storage release loop at the end of this routine.
 */
	if (firsttime) {
		ptr[0] = unops;
		ptr[1] = binops;
		ptr[2] = postops;
		for (i = 0; i < 3; i++)
			do {
				char   *fptr = fld;
				while ((bc = *ptr[i]++) != NULL) {
					if (isspace (bc))
						continue;
					if (index ("{,}", bc)) {
						*fptr = NULL;
						break;
					}
					*fptr++ = bc;
				}
				if (bc == '{') {
					p++;
					continue;
				}
				LINK (o, next_o, olist, operator);
				(void) strncpy (o -> op, fld, OPMAX);
				o -> prec = p;
				if (i == 0) {
					o -> otype = UNOP;
					o -> nopnds = 1;
				}
				if (i == 1) {
					o -> otype = BINOP;
					o -> nopnds = 2;
				}
				if (i == 2) {
					o -> otype = POSTOP;
					o -> nopnds = 1;
				}
			} while (strlen (ptr[i]));
		firsttime = 0;
	}

	os[0] = NULL;

	while (strlen (expr)) {

		if (*expr == '(') {
			S[++top].prec = 99;
			(void) strcpy (S[top].op, "(");
			expr++;
			prev = LPN;
			continue;
		}

		if (*expr == ')') {
			if (prev & OPND)
				(void) strcat (os, "$0,");
			while (top && strcmp (S[top].op, "("))
				(void) strcat (os, S[top--].op);
			if (top)
				top--;
			expr++;
			prev = RPN;
			continue;
		}

		lmatch = 0;
		FORLIST (o, next_o, olist)
			if (!strncmp (expr, o -> op, (i = strlen (o -> op)))) {
		/* UNOPS may follow only INIT, UNOP, BINOP, or LPN */
			if ((o -> otype & UNOP) 
			    && !(prev & (INIT | UNOP | BINOP | LPN)))
				continue;
			if (i > 1 && isalnum (*(expr + i)))
				continue;
			if (i > lmatch) {
				lmatch = i;
				oo = o;
			}
		}

		if (!lmatch) {
			if (isspace (*expr)) {
				expr++;
				continue;
			}
			else {
				os[i = strlen (os)] = *expr++;
				os[i + 1] = NULL;
				prev = OPND;
				continue;
			}
		}

		if (lmatch) {
			if (prev & OPND)
				(void) strcat (os, "$0,");
			expr += strlen (oo -> op);

			while (top && (S[top].prec <= oo -> prec))
				(void) strcat (os, S[top--].op);
			S[++top] = *oo;
			(void) sprintf (fld, "$%d,", oo -> nopnds);
			(void) strcat (S[top].op, fld);
			prev = oo -> otype;
			continue;
		}
	}
	if (prev & OPND)
		(void) strcat (os, "$0,");
	while (top)
		(void) strcat (os, S[top--].op);
	return (os);
}
