#include <stdio.h>
#include <math.h>
#include <carl/carl.h>

main(narg,argv) int narg; char *argv[]; 
{
float rms = 0;
float input;
long n = 0;
int otty = isatty(1), putout = 0;
char ch;
int dBrms = 0;

if (isatty(0))
	usage(1);

while ((ch = crack(narg, argv, "ehd|", 0)) != NULL) {
	switch (ch) {
		case 'e':	
			putout++;
			break;
		case 'd':
			dBrms++;
			break;
		case 'h':
			usage(0);
		default:
			usage(1);
	}
}

if (otty && putout) {
	fprintf(stderr, "rms: for -e, output must be file or pipe.\n");
	exit(1);
}

while ( getfloat(&input) > 0) {
	if (putout)
		putfloat(&input);
	rms += input * input;
	n++;
}

if (putout)
	flushfloat();
fprintf(putout ? stderr : stdout, "%f\n", 
	(double)(dBrms ? 10.0*log10(rms/n) : sqrt(rms/n)));
}

usage(x)
	int x;
{
	fprintf(stderr, "%s%s%s",
		"usage:\n",
		"\trms [-dB] < floatsams > rms (ASCII numbers)\n",
		"\trms -e [-dB]  < floatsams > floatsams, and >& rms\n");
	fprintf(stderr,"input must be a file or pipe of floatsams.\n");
	exit(x);
}
