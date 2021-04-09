#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

#define NBIN 16
#define FABS(n)(n<0?-n:n)
int bin[NBIN];
float thresh[NBIN];

main(narg,argv) int narg; char *argv[]; 
{
register int i, j, max, nsamp = 0;
int otty = isatty(1), putout = 0;
float input; 
register float dc = 0.0, rms = 0.0, peak = 0.0;

if (isatty(0)) {
	fprintf(stderr, "%s%s%s",
		"usage:\n",
		"\thist < floatsams > histogram (ASCII display)\n",
		"\thist -e < floatsams > floatsams, and >& historgram\n");
	fprintf(stderr,"input must be a file or pipe of floatsams.\n");
	exit(1);
}
if (narg > 1 && argv[1][0] == '-' && argv[1][1] == 'e') {
	if (!otty) 
		putout++;
	else {
		fprintf(stderr, "hist: for -e, output must be file or pipe.\n");
		exit(1);
	}
}
		
for (i=0; i<NBIN; i++)
	thresh[i] = pow(2.0, (float) -i );
thresh[NBIN-1] = -0.000001;

while ( getfloat(&input) > 0) {
	if (putout)
		putfloat(&input);
	nsamp++;
	dc += input;
	rms += input * input;
	if (FABS(input) > peak ) 
		peak = FABS(input);
	if (input < 0.0) 
		input = -input;
	for (i=NBIN-1; i>=0; i--)
		if (input < thresh[i] ) { 
			bin[i+1]++; break; 
		}
}
if (putout)
	flushfloat();
for (max=i=0; i<NBIN; i++)
	if (bin[i] > max)
		max = bin[i];

if( nsamp > 0 ) {
    if( peak == 0.0 ) {
	    fprintf(putout?stderr:stdout,"All-Zeros: " ) ;
	    goto lastprint ;
    }
    for (i=0; i<NBIN; i++){
	    fprintf(putout?stderr:stdout, "> %8.5f (= %6.1fdB) = %6d",thresh[i],
		    20*log10(thresh[i]>0?thresh[i]:1.e-6), bin[i]);
	    fprintf(putout?stderr:stdout," |");
	    for (j=0; j < (int) 40.0*bin[i]/max; j++)
		    fprintf(putout?stderr:stdout, "*");
	    fprintf(putout?stderr:stdout,"\n");
    }
    dc /= nsamp; 
    fprintf(putout?stderr:stdout,
	    "Mean = %f",dc ) ;
    if( dc != 0.0 )
	fprintf(putout?stderr:stdout,
	    " (= %.1fdB)\n", 20.0*log10(dc>0?dc:-dc));
     else
	fprintf(putout?stderr:stdout,"\n");
    fprintf(putout?stderr:stdout,
	    "Peak = %f (= %.1fdB)\n",peak, 20.0*log10(peak>0?peak:-peak));
    fprintf(putout?stderr:stdout,
	    "RMS = %f (= %.1fdB)\n", sqrt(rms/nsamp), 10.0*log10(rms/nsamp));
}
lastprint:
fprintf(putout?stderr:stdout,
	"%d samples\n", nsamp);

exit(0);
}
