#include <stdio.h> 
#include <carl/fun.h>
#include <carl/carl.h>
#include <carl/sndio.h>
#include <carl/defaults.h>

#define SRATE FDEFSR
#define DLT (1.0)
#define IGNORE 1
#define DONT_IGNORE 0

extern int arg_index;	/* from crack(); */
extern char *arg_option;

int bug;

main(argc, argv)
	char **argv;
{
	FUN *f;
	int n, cnt = 0, otty = isatty(1);
	float zlag=.1*SRATE, dur=1.0;
	int scnt = dur * SRATE;
	char c, crack();
	float sfexpr(), srate = SRATE, *zbuf, input; 
	int maxdlt = DLT*SRATE;
	float rind; 
	int wind = 0;

	while ((c = crack(argc, argv, "R|", IGNORE)) != NULL) {
		if (c == 'R') {
			srate = sfexpr(arg_option, 1.0);
			maxdlt = srate*DLT;
			zlag = srate * .1;
		}
	}
	arg_index = 0;
	while ((c = crack(argc, argv, "D|z|R|d|bh", DONT_IGNORE)) != NULL) {
		switch (c) {
			case 'b': bug++; break;
			case 'D': maxdlt = sfexpr(arg_option, srate); break;
			case 'z': zlag = sfexpr(arg_option, srate); break;
			case 'd': dur = sfexpr(arg_option, 1.0); break;
			case 'R':
				break;
			default: 
			case 'h':
				usage(0);
		}
	}
	
	if (arg_index < argc ) {
		if ((f = rautoxfun(argv[arg_index])) == NULL) {
			fprintf(stderr, "failed reading function file %s\n",
				arg_option);
			exit(1);
		}
	} else {
		fprintf(stderr, "no function file specified.\n");
		exit(1);
	}
		

	scnt = dur * srate;
	zbuf = (float *) malloc(sizeof(float) * maxdlt);

	while (n = getfloat(&input) > 0) {
		float output, inc, zdelay(), incrfun();

		inc = incrfun(f, cnt, scnt);
		output = zdelay(input, inc, zlag, maxdlt, zbuf, &rind, &wind);
		if (otty) 
			printf("%d:\t%6.3f\t%6.3f\n", cnt, inc, output);
		else 
			putfloat(&output);
		cnt++;
	}
	if (!otty) 
		flushfloat();
}

usage(x)
{
fprintf(stderr, "%s%s%s%s%s%s%s%s",
"usage: zdelay [flags] function_file < floatsams > floatsams\n",
"flags: (defaults)\n",
" -RN\tset sample rate to N (",
DEFSR,
"Hz)\n",
" -DN\tset maximum size of delay table in seconds to N (.1)\n",
" -zN\tset starting delay offset in seconds to N (.05)\n",
" -dN\tset duration of input floatsam file to N\n"
);
exit(x);
}
