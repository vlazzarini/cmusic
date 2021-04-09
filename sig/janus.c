#include<stdio.h>
#include<math.h>
#include <carl/carl.h>
#include <carl/defaults.h>

extern int arg_index;
extern char *arg_option;
extern char crack();
extern float sfexpr();

double pi, piovr2;

main(argc, argv)
	char **argv;
{
	extern double atof();
	int otty = isatty(1);
	char ch;
	long int beg, end, cnt=0;
	float srate=FDHISR, output, input, janbeg(), janend();

	pi = 4.0 * atan(1.0);
	piovr2 = 2.0 * atan(1.0);

	while ((ch = crack(argc, argv, "h", 1)) != NULL)
		if (ch == 'h') 
			janushelp(0);

	if (getheader(stdin) != NULL) 
		{
		register char *pval;

		if ((pval = getprop(stdin, H_SRATE)) != NULL)
			srate = atof(pval);
		}

	arg_index = 0;
	while ((ch = crack(argc, argv, "R|", 1)) != NULL)
		{
		if (ch == 'R') 
			{ srate = sfexpr(arg_option, 1.0); break; }
		}
	beg = .05 * srate;
	end = .05 * srate;
	arg_index = 0;
	while ((ch = crack(argc, argv, "R|b|e|h", 0)) != NULL)
		{
		switch (ch) {
			case 'R': break;
			case 'b': beg = sfexpr(arg_option, srate); break;
			case 'e': end = sfexpr(arg_option, srate); break;
			case 'h': janushelp(0);
			default:  janushelp(1);
			}
		}


	if ((cnt = setbufup(end)) != end)
		{
		fprintf(stderr, "janus: expected %d samples, but only got %d\n",
			end, cnt);
		exit(1);
		}

	cnt = 0;
	while (bufup(&input) > 0)
		{
		if (cnt < beg)
			output = janbeg(input, beg);
		else 
			output = input;
		if (otty) 
			printf("%d:\t%f\n", cnt, output);
		else 
			putfloat(&output);
		cnt++;
		}

	while (bufend(&input) > 0)
		{
		output = janend(input, end);
		if (otty) 
			printf("%d:\t%f\n", cnt++, output);
		else 
			putfloat(&output);
		}

	if (!otty) 
		flushfloat();
	exit(0);
}

float janbeg(in, beg)
	float in; long int beg;
{
	static long int cnt = 0;
	float rtn;

	return(in * 0.5 * (cos(pi * (float) cnt++ / beg + pi) + 1.0));
}
	
float janend(in, end)
	float in; long int end;
{
	static long int cnt = 0;

	return(in * 0.5 * (sin(pi * (float) cnt++ / end + piovr2) + 1.0));
}

janushelp(ex)
	int ex;
{
fprintf(stderr, "%s%s%s%s%s%s%s%s",
"usage: janus [flags] < floatsams > floatsams\n",
" flags:\n",
" -bN\tscale beginning until time N (default= .05sec)\n",
" -eN\tstart ending scaling at time N from end of file (default= .05sec)\n",
" -RN\tset sampling rate for time calculations\n",
"\t(if no header, default= ",
DHISR,
" Hz), flag overrides header)\n"
);
exit(ex);
}

float *buf;
long int len, len1;

setbufup(cnt)
	long cnt;
{
	register int i=0;
	float input;

	len = len1 = cnt;	/* save global lengths */
	buf = (float *) malloc(sizeof(float) * cnt);
	if (buf == NULL) {
		fprintf(stderr, "janus: out of memory!\n");
		exit(1);
	}
	while (i < cnt && (getfloat(&input) > 0))
		buf[i++] = input;
	return(i);
}
	
long int ptr = 0;

bufup(samp)
	float *samp;
{
	register int rtn;

	if (ptr >= len) 		/* wraparound */
		ptr = 0;
	*samp = buf[ptr];
	if (getfloat(&buf[ptr]) > 0) {
		ptr++;
		return(1);
	} else 
		return(0);
}

bufend(samp)
	float *samp;
{
	if (ptr >= len) ptr = 0;
	*samp = buf[ptr++];
	return(len1--);
}
