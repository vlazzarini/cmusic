#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

main(narg,argv) int narg; char *argv[]; 
{
    float input, output;
    float value;

    if(narg != 2){
	fprintf(stderr, "usage: offset value\n");
	exit(-1);
    } else value = expr(argv[1]);
    if(exprerr)
	{fprintf(stderr, "Illegal value expression '%s'\n",argv[1]); exit(-1);}
    if(isatty(0)){
	fprintf(stderr,"input must be a file or pipe.\n");
	exit(-1);
    }
    if(isatty(1)){
	while ( getfloat(&input) > 0){
	    printf("%f\n",input+value); 
	}
    } else {
	while ( getfloat(&input) > 0){
	    output = input+value;
	    putfloat(&(output));
	}
    flushfloat();
    }
}
