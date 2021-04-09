#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

main(narg,argv) int narg; char *argv[]; 
{
    float input, output;
    float dur, rise, fall;
    int samp;

    if(narg != 2){
fprintf(stderr, "usage: window duration rise fall (times in samples)\n");
	exit(-1);
    } else dur = expr(argv[1]);
    if(exprerr)
       {fprintf(stderr,"Illegal duration expression '%s'\n",argv[1]);exit(-1);}
    rise = expr(argv[2]);
    if(exprerr)
       {fprintf(stderr,"Illegal rise expression '%s'\n",argv[2]);exit(-1);}
    fall = expr(argv[3]);
    if(exprerr)
       {fprintf(stderr,"Illegal fall expression '%s'\n",argv[3]);exit(-1);}
    if(isatty(0)){
	fprintf(stderr,"input must be a file or pipe.\n");
	exit(-1);
    }

/* For efficiency, separate terminal and pipe output cases */

    samp = 0;
    if(isatty(1)){
	while ( getfloat(&input) > 0){
	    if(samp < rise)input *= samp/rise;
	    if( (dur-samp) < fall)input *= (dur-samp)/fall;
	    printf("%f\n",input); 
	    samp++;
	}

    } else {

	while ( getfloat(&input) > 0){
	    if(samp < rise)input *= samp/rise;
	    if( (dur-samp) < fall)input *= (dur-samp)/fall;
	    putfloat(&(input));
	    samp++;
	}
	flushfloat();
    }
}
