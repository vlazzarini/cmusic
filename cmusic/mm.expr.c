/* cmusic's private version of expr (knows about symbols, such as p5,
   and knows the current values of sampling rate, function length, etc. */
#include <stdlib.h>
#include <string.h>
#include "mm.head.h"

#define LRPN 1000
#define UNOPS "{sin,cos,atan,ln,exp,floor,abs,rand,sqrt}{-}"
#define BINOPS "{^,%}{*,/}{+,-}"
#define POSTOPS "{S,Hz,dB,K,ms,Deg,sec,MM,IS}"

float expr(string) char *string;{
    char *polish(), *rpnp, rpn[LRPN], *it, item[1024], sym[1024];
    char *substr[LRPN];
    char *lb, *rb, *index(), ssstr[LRPN];
    int i, blev, nops, n, top = 0;
    float tmpx;
    int subscriptval;
    double stack[LRPN];

    rpnp = rpn;
/* Strip out subscript expressions */
    while(lb = rb = index(string,'[')){
	blev = 0;
	while(*(++rb)){
	    if(*rb == '[')blev++; 
	    if(*rb == ']' && blev == 0)break;
	    if(*rb == ']')blev--;
	    if(blev < 0){
		Error++;
		PROUT("Unbalanced []'s\n",blev);
		return(0.0);
	    }
	}
	substr[0] = '\0';
	strncat(substr,lb+1,rb-lb-1);
	subscriptval = expr(substr);
	sprintf(ssstr,"%d",subscriptval);
	ssinsert(string,lb,rb,ssstr);
    }
    rpn[0] = '\0';
    strncat((char *)rpn,polish(string,UNOPS,BINOPS,POSTOPS),LRPN);

    while(strlen(rpnp)){
	sfield(&rpnp, item, "", ",");
	it=item;
	sfield(&it, sym, "", "$");
	nops = atoi(it);
	if(!nops){
	    if(sym[0]=='p'){
		if(sym[1] != '[')n = atoi(&sym[1]);
		 else{
		    sym[strlen(sym)-1] = '\0';
		    n = expr(&sym[2]);
		 }
		if(n > Pmax){
		    Pmax = n;
		    EXTEND(Pp, Params, Pmax+1, float);
		    PROUT("%d note parameters (due to expression reference).\n",
			Pmax+1);
		}
		stack[++top] = *(Params + n);
		continue;
	    }
	    if(sym[0]=='v'){
		n = atoi(&sym[1]);
		if(n > Vmax){
		    Vmax = n;
		    EXTEND(Vp, Vars, Vmax+1, float);
		   PROUT("%d static variables (due to expression reference).\n",
			Vmax+1);
		}
		stack[++top] = *(Vars + n);
		continue;
	    }
	    for (i = strlen(sym)-1; i >=0; i--)
	     if(isdigit( *(sym+i)) || *(sym+i) == '.' || *(sym+i) == 'x'
	      || *(sym+i) == 'X') continue;
	       else{
		    Error++;
		    PROUT("\nEXPR: Illegal symbol: '%s'\n",sym);
		    return( (float) 0.0 );
	       }
	    if(!index(sym,'.') && *sym == '0'){
		if( *(sym+1) == 'x' || *(sym+1) == 'X' ) 
		    sscanf(sym+2, "%x", &n);
		else sscanf(sym, "%o", &n);
		    stack[++top] = n;
	    } else if(!sscanf(sym,"%f", &tmpx)){
		Error++;
		PROUT("\nEXPR: Illegal expression: '%s'\n",string);
		return( (float) 0.0 );
	    } else {
		/* we should check for eof as well */
		stack[++top] = tmpx;
	    }

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
	       stack[top] = (double) random()/(double) 0x7fffffff * stack[top]; 
		continue;}
	    }
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
		stack[top-1] = (long) stack[top-1] % (long) stack[top];
		top--;
		continue;
	    }
	    if(!strcmp(sym,"Hz") && nops == 1){
		stack[top] = stack[top] * Lfuncs / Srate;
		continue;
	    }
	    if(!strcmp(sym,"sec") && nops == 1){
		stack[top] = Lfuncs / (stack[top] * Srate);
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
	    if(!strcmp(sym,"ms") && nops == 1){
		stack[top] = ceil(stack[top] * Srate / 1000.0);
		continue;
	    }
	    if(!strcmp(sym,"S") && nops == 1){
		stack[top] /= (double) Srate;
		continue;
	    }
	    if(!strcmp(sym,"MM") && nops == 1){
		stack[top] = 60.0 / stack[top];
		continue;
	    }
	    if(!strcmp(sym,"Deg") && nops == 1){
		stack[top] = stack[top] / 360.0 * TWOPI;
		continue;
	    }
	    if(!strcmp(sym,"IS") && nops == 1){
		if(stack[top]<1.0){stack[top] = 0.0; continue;}
		n = stack[top];
		stack[top] = 0;
		for(i = 1; i <= n; i++)stack[top] += 1.0/i;
		continue;
	    }
	}
	if(top != 1){
	    Error++;
printf("that one top = %d\n", top);
	    PROUT("\nEXPR: Illegal expression: '%s'\n",string);
	    return( (float) 0.0 );
	}
	if(Verbose || Listfile || Barefile){ float v = (float) stack[top];
	    sprintf(item,"%f",v);
	    sqz0s(item);	/* Squeeze trailing fractional zeros */
	    if(Verbose)fprintf(stderr," {=%s} ", item);
	    if(Listfile)fprintf(Listfile," {=%s} ", item);
	    if(Barefile)fprintf(Barefile," {=%s} ", item);
	}
	return( (float) stack[top]);
}
ssinsert(s,a,b,new) char *s, *a, *b, *new;{
 char stemp[LRPN];
    stemp[0] = '\0';
    strncat(stemp,s,a-s);
    strcat(stemp,new);
    strcat(stemp,(b+1));
    strcpy(s,stemp);
    return;
}
