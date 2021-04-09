/* General-purpose expr (NOT used by cmusic, but can be used by any
   program for command line expression arguments, for example
   (see man 3X expr or help expr)
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#define LRPN 1000
#define UNOPS "{sin,cos,atan,ln,exp,floor,abs,rand,sqrt}{-}"
#define BINOPS "{^,%}{*,/}{+,-}"
#define POSTOPS "{dB,K,k,Deg,invs,MM}"
#define TWOPI (8.0 * atan(1.0) )

int exprerr = 0;	/* error flag */

float expr(string) char *string;{
    char *polish(), *rpnp, rpn[LRPN], *it, item[40], sym[40];
    int nops, i, n, top = 0;
    double stack[LRPN];
    float tmpx;

    exprerr = 0;
    rpnp = rpn;
    strncpy(rpn,polish(string,UNOPS,BINOPS,POSTOPS),LRPN);

    while(strlen(rpnp)){
	sfield(&rpnp, item, "", ",");
	it=item;
	sfield(&it, sym, "", "$");
	nops = atoi(it);
	if(!nops){
	    if(!index(sym,'.') && *sym == '0'){
		if( *(sym+1) == 'x' || *(sym+1) == 'X' ){
		    sscanf(sym+2, "%x", &n);
		} else {
		    sscanf(sym, "%o", &n);
		}
		    stack[++top] = n;
	    } else {
		top++;
		if(!sscanf(sym,"%f",&tmpx))
		    exprerr++;
		else
		    stack[top] = tmpx;
	    }
	    continue;
	}
	if(nops){
	    if(!strcmp(sym,"sin")){stack[top] = sin(stack[top]); continue;}
	    if(!strcmp(sym,"cos")){stack[top] = cos(stack[top]); continue;}
	    if(!strcmp(sym,"atan")){stack[top] = atan(stack[top]); continue;}
	    if(!strcmp(sym,"ln")){stack[top] = log(stack[top]); continue;}
	    if(!strcmp(sym,"exp")){stack[top] = exp(stack[top]); continue;}
	    if(!strcmp(sym,"floor")){stack[top] = floor(stack[top]); continue;}
	    if(!strcmp(sym,"abs")){stack[top] = fabs(stack[top]); continue;}
	    if(!strcmp(sym,"sqrt")){stack[top] = sqrt(stack[top]); continue;}
	    if(!strcmp(sym,"rand")){
		stack[top] = (double) rand()/(double) 0x7fffffff * stack[top]; 
		continue;}
	    if(!strcmp(sym,"-") && nops == 1){
		stack[top] = -stack[top];
		continue;
	    }
	    if(!strcmp(sym,"-") && nops == 2){
		stack[top-1] = stack[top-1] - stack[top];
		top--;
		continue;
	    }
	    if(!strcmp(sym,"+") && nops == 2){
		stack[top-1] = stack[top-1] + stack[top];
		top--;
		continue;
	    }
	    if(!strcmp(sym,"*") && nops == 2){
		stack[top-1] = stack[top-1] * stack[top];
		top--;
		continue;
	    }
	    if(!strcmp(sym,"/") && nops == 2){
		stack[top-1] = stack[top-1] / stack[top];
		top--;
		continue;
	    }
	    if(!strcmp(sym,"^") && nops == 2){
		stack[top-1] = pow( stack[top-1], stack[top]);
		top--;
		continue;
	    }
	    if(!strcmp(sym,"%") && nops == 2){
		stack[top-1] = (int) stack[top-1] % (int) stack[top];
		top--;
		continue;
	    }
	    if(!strcmp(sym,"dB") && nops == 1){
		stack[top] = pow( (double) 10.0, stack[top]/20.);
		continue;
	    }
	    if(!strcmp(sym,"K") && nops == 1){
		stack[top] = stack[top] * 1024.0;
		continue;
	    }
	    if(!strcmp(sym,"k") && nops == 1){
		stack[top] = stack[top] * 1000.0;
		continue;
	    }
	    if(!strcmp(sym,"Deg") && nops == 1){
		stack[top] = stack[top] / 360.0 * TWOPI;
		continue;
	    }
	    if(!strcmp(sym,"invs") && nops == 1){
		if(stack[top]<1.0){stack[top] = 1.0; continue;}
		n = stack[top];
		stack[top] = 0;
		for(i = 1; i <= n; i++)stack[top] += 1.0/i;
		continue;
	    }
	    if(!strcmp(sym,"MM") && nops == 1){
		stack[top] = 60.0 / stack[top];
		continue;
	    }
	    }
	    exprerr++;
	}
	if(top != 1)exprerr++;
	return( (float) stack[top]);
}

sfield(input,string,iglist,brklist) char **input, *string, *iglist, *brklist;{
    int leading, c;
    leading = 1;
    while( (c = *(*input)++) != (int)NULL){
	if(leading && c==' ')continue;
	if(index(iglist,c))continue;
	if(!index(brklist,c)){*string++ = c; leading = 0; continue;}
	    else {*string = (int)NULL; return(c);}
    }
    return((int)NULL);
}
