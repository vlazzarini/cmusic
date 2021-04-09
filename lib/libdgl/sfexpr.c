/*
 * sfexpr - similar to expr() routine from libfrm.a, but optimized for
 * parsing command line for csound.  No major differences, some
 * features left out, such as sin(), ln(), etc.  Postoperators added:
 * s - seconds, S - samples, K - 1024, ms - milliseconds, m - minutes,
 * dB. 
 */

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <carl/sndio.h>
#include <math.h>

#define LRPN 200
#define UNOPS "{-}"
#define BINOPS "{^,%}{*,/}{+,-}"
#define POSTOPS "{dB,K,k,S,s,m,ms}"
#define TWOPI (8.0 * atan(1.0) )

float sfexpr(string, timefac)  
	char *string; float timefac;
{
    char *polish(), *rpnp, rpn[LRPN], *it, item[40], sym[40];
    int nops, n, top = 0;
    double stack[LRPN];
    float tmpx;

    rpnp = rpn;
    strncpy(rpn,polish(string,UNOPS,BINOPS,POSTOPS),LRPN);

    while(strlen(rpnp))
	{
	sffield(&rpnp, item, "", ",");
	it=item;
	sffield(&it, sym, "", "$");
	nops = atoi(it);
	if (!nops)	/* it is a constant number */
	    {
	    if(!index(sym,'.') && *sym == '0') /* no '.' and '0' prefix */
		{
		if( *(sym+1) == 'x' || *(sym+1) == 'X' ) 
		    sscanf(sym+2, "%x", &n);
		else sscanf(sym, "%o", &n);
		    stack[++top] = n;
	        } 
	    else {
		top++;
		sscanf(sym,"%f",&tmpx);
		stack[++top] = tmpx;
	    }
	    stack[top] *= timefac;
	    continue;
	    }
	/* from here on, deal with operators only */
	if (!strcmp(sym,"-") && nops == 1)
	    {
	    stack[top] = -stack[top];
	    continue;
	    }
	else
	if (nops == 2)
	    {
	    switch(sym[0])
		{
		case '-':
		    stack[top-1] = stack[top-1] - stack[top];
		    break;
		case '+':
		    stack[top-1] = stack[top-1] + stack[top];
		    break;
		case '*':
		    stack[top-1] = stack[top-1] * stack[top];
		    break;
		case '/':
		    stack[top-1] = stack[top-1] / stack[top];
		    break;
		case '^':
		    stack[top-1] = pow(stack[top-1], stack[top]);
		    break;
		case '%':
		    stack[top-1] = (int) stack[top-1] % (int) stack[top];
		}
	    top--;
	    }
	else
	if (nops == 1)
	    {
	    if(!strcmp(sym,"dB")){
		stack[top] /= timefac;
		stack[top] = pow( (double) 10.0, stack[top]/20.);
		continue;
		}
	    if(!strcmp(sym,"K")){
		stack[top] *= 1024.0;
		continue;
		}
	    if(!strcmp(sym,"k")){
		stack[top] *= 1000.0;
		continue;
		}
	    if(!strcmp(sym,"S")){
		stack[top] /= timefac;
		continue;
		}
	    if(!strcmp(sym,"ms")){
		stack[top] *= 0.001;
		continue;
		}
	    if(!strcmp(sym,"m")){
		stack[top] *= 60.0;
		continue;
		}
	    if(!strcmp(sym,"s")){
		continue;
		}
	    }
	}
    return( (float) stack[top]);
    }

sffield(input, string, iglist, brklist) 
	char **input, *string, *iglist, *brklist;
{
	int leading = 1, c;
	while ((c = *(*input)++) != NULL) {
		if (leading && c==' ')
			continue;
		if (index(iglist,c))
			continue;
		if (!index(brklist,c)){
			*string++ = c; 
			leading = 0; 
			continue;
		}
		else {
			*string = NULL; 
			return;
		}
	}
	 return; 
}
