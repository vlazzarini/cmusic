#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

main(narg,argv) int narg; char *argv[]; 
{
    float input, last;
    float thresh;
    long eof, n, c, nchan, channel;

    if(narg != 2){
	fprintf(stderr, "CLICK Usage: click threshold\n");
	exit(-1);
    } 
    thresh = expr(argv[1]);
    if(exprerr)
       {fprintf(stderr,"Illegal threshold expression '%s'\n",argv[1]);exit(-1);}
    if(isatty(0)){
	fprintf(stderr,"input must be a file or pipe.\n");
	exit(-1);
    }
    while ( getfloat(&input) > 0){
	if( fabs(input-last) > thresh) printf("%d %f\n",n,fabs(input-last));
	last = input;
	n++;
    }
}
