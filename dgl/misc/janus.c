#include<stdio.h>
#include<carl/carl.h>
#include<math.h>

extern int arg_index;
extern char *arg_option;
extern char crack();
extern float sfexpr();

double pi, piovr2;

main(argc, argv)
	char **argv;
{
	int otty = isatty(1);
	char ch;
	long int beg, end, cnt=0;
	float srate=(48*1024), output, input, janbeg(), janend();

	pi = 4.0 * atan(1.0);
	piovr2 = 2.0 * atan(1.0);
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
			case 'h':
			default:  janushelp();
			}
		}
	
	if (setbufup(end) != end)
		{
		fprintf(stderr, "janus: not enough samples\n");
		exit(-1);
		}

	while (bufup(&input) > 0)
		{
		if (cnt < beg) 
			output = janbeg(input, beg);
		else 
			output = input;
		if (otty) printf("%d:\t%f\n", cnt, output);
		else putfloat(&output);
		cnt++;
		}

	while (bufend(&input) > 0)
		{
		output = janend(input, end);
		if (otty) printf("%d:\t%f\n", cnt++, output);
		else putfloat(&output);
		}

	if (!otty) flushfloat();
	}

float janbeg(in, beg)
	float in; long int beg;
{
	static long int cnt = 0;
	return(in * 0.5 * (cos(pi * (float) cnt++/beg + pi) + 1.0));
	}
	
float janend(in, end)
	float in; long int end;
{
	static long int cnt = 0;
	return(in * 0.5 * (sin(pi * (float) cnt++/end + piovr2) + 1.0));
	}

janushelp()
{
fprintf(stderr, "%s%s%s%s%s",
"usage: janus [flags] < floatsams > floatsams\n",
" flags:\n",
" -bN\tscale beginning until time N (default= .05sec)\n",
" -eN\tstart ending scaling at time N from end of file (default= .05sec)\n",
" -RN\tset sampling rate for time calculations (default= 48K)\n"
);
exit(-1);
}

float *buf;
long int len, len1;

setbufup(cnt)
	long int cnt;
{
	int i=0;
	float input;

	len = len1 = cnt;
	buf = (float *) malloc(sizeof(float) * cnt);
	while (cnt-- && (getfloat(&input) > 0))
		buf[i++] = input;
	return(i);
	}
	
long int ptr = 0;

bufup(samp)
	float *samp;
{
	if (ptr >= len) ptr = 0;
	*samp = buf[ptr];
	return(getfloat(&buf[ptr++]));
	}

bufend(samp)
	float *samp;
{
	if (ptr >= len) ptr = 0;
	*samp = buf[ptr++];
	return(len1--);
	}
