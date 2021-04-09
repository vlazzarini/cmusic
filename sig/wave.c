/* %M%	%I%	(CARL)	%G%	%U% */
#include <stdio.h>
#include <sys/types.h>
#include <carl/sndio.h>
#include <math.h>
#include <carl/carl.h>
#include <carl/defaults.h>

int otty, header = 1;

main(argc, argv)
	int argc; char **argv;
{
	float sfexpr(), sr = FDLOSR, fc = 440.0, amp = 1.0, dur = 1.0;
	char ch, waveform = 's';
	long nsamps, silence = 0;
	float fsilence;
	float fcsr;

	otty = isatty(1);

	if (argc == 1 && otty) 
		help();

	while ((ch = crack(argc, argv, "hHR|r|f|w|a|d|T|S|", 0)) != NULL)
		{
		switch(ch)
			{
			case 'H':
				header = 0;
				break;
			case 'h': 
			default:
				help();
			case 'R':
			case 'r':
				sr = sfexpr(arg_option, 1.0);
				break;
			case 'f':
				fc = sfexpr(arg_option, 1.0);
				break;
			case 'w':
				waveform = *arg_option;
				break;
			case 'a':
				amp = sfexpr(arg_option, 1.0);
				break;
			case 'd':
			case 'T':
				dur = sfexpr(arg_option, sr)/sr;
				break;
			case 'S':
				fsilence = sfexpr(arg_option, sr)/sr;
			}
		}
	if (otty)
		header = 0;

	nsamps = sr * dur;
	fcsr = fc / sr;
	silence = fsilence * sr;

	if (header)
		if (whead(sr, fc, amp, dur*sr)) {
			fprintf(stderr, "can't write header\n");
			exit(1);
		}

	switch (waveform)
		{
		case 's': 
			sinewave(nsamps, amp, fcsr);
			break;
		case 'd':
			dc(nsamps, amp);
			break;
		case 'S':
			dc(nsamps, 0.0);
			break;
		case 'q': 
			squarewave(nsamps, amp, fcsr);
			break;
		case 't': 
			trianglewave(nsamps, amp, fcsr);
			break;
		case 'r': 
			rampwave(nsamps, amp, fcsr);
			break;
		case 'i': 
			impulse(nsamps, amp, fcsr);
			break;
		case 'p': 
			pulsetrain(nsamps, amp, fcsr);
			break;
		case 'u':
			whitenoise(nsamps, amp, fcsr);
			break;
		default:
			fprintf(stderr, "wave: waveform %c not implemented.\n",
				waveform);
			exit(1);
		}

	while (silence--)
		{
		float output = 0.0;
		if (otty)
			printf("%d\t%f\n", ++nsamps, 0.0);
		else
			putfloat(&output);
		}

	if (!otty)
		flushfloat();
	exit(0);
	}

sinewave(ns, a, fcsr)
	long ns; float a, fcsr;
{
	float output; 
	register float twopi = 8.0 * atan(1.0);
	register long i;
	for (i = 0; i < ns; i++)
		{
		output = a * sin(twopi * fcsr * i);
		if (otty)
			printf("%d:\t%f\n", i, output);
		else
			putfloat(&output);
		}
	}

dc(ns, a)
	long ns; float a;
{
	float output;
	register long i;
	output = a;
	for (i = 0; i < ns; i++)
		{
		if (otty)
			printf("%d:\t%f\n", i, output);
		else
			putfloat(&output);
		}
	}

squarewave(ns, a, fcsr) 
	register long ns; register float a, fcsr;
{
	float output;
	register long i;
	register float inc = 0.0;
	for (i = 0; i < ns; i++)
		{
		output = a * ((inc >= .5) ? -1.0 : 1.0);
		inc += fcsr;
		if (inc >= 1.0) inc = 0.0;
		if (otty)
			printf("%d:\t%f\n", i, output);
		else
			putfloat(&output);
		}
	}

trianglewave(ns, a, fcsr) 
	register long ns; register float a, fcsr;
{
	register float inc = 0.0;
	float output = 0.0;
	register long i, up = 1;
	fcsr *= 4.0;
	for (i = 0; i < ns; i++)
		{
		output = a * inc;
		if (otty)
			printf("%d:\t%f\n", i, output);
		else
			putfloat(&output);
		if (up)
			{
			inc += fcsr;
			if (inc >= 1.0) { up--; inc = 1.0; }
			}
		else
			{
			inc -= fcsr;
			if (inc <= -1.0) { up++; inc = -1.0; }
			}
		}
	}

rampwave(ns, a, fcsr) 
	register long ns; register float a, fcsr;
{
	float output = 0.0;
	register float inc = 0.0;
	register long i;
	fcsr *= 2.0;
	for (i = 0; i < ns; i++)
		{
		output = inc * a;
		if (otty)
			printf("%d:\t%f\n", i, output);
		else
			putfloat(&output);
		inc += fcsr;
		if (inc >= 1.0) inc = -1.0;
		}
	}

impulse(ns, a, fcsr)
	register long ns; register float a;
{
	float output = a;
	register long i;
	for (i = 0; i < ns; i++)
		{
		if (otty)
			printf("%d:\t%f\n", i, output);
		else
			putfloat(&output);
		output = 0.0;
		}
	}

pulsetrain(ns, a, fcsr)
	register long ns; register float a, fcsr;
{
	float output;
	register long i;
	register float inc = 0.0;
	for (i = 0; i < ns; i++)
		{
		output = a * ((inc == 0.0) ? 1.0 : 0.0);
		if (otty)
			printf("%d:\t%f\n", i, output);
		else
			putfloat(&output);
		inc += fcsr;
		if (inc >= 1.0) inc = 0.0;
		}
	}

whitenoise(ns, a, fcsr)
	register long ns; register float a;
{
	register long i;
	register long ran;
	float output, divisor = pow((double)2.0,(double)30.0)-1.0;
	for (i = 0; i < ns; i++)
		{
		ran = rand();
		output = a * (ran / divisor) - 1.0;
		if (otty)
			printf("%d:\t%f\n", i, output);
		else
			putfloat(&output);
		}
	}

help() 
{
printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
"flags:\n",
"-RN = set sampling rate to N (",
DEFSR,
")\n",
"-fN = set frequency to N (440)\n",
"-wC = select waveform C (s) from the following\n",
"    s = sine\n",
"    r = ramp\n",
"    q = square\n",
"    t = triangle\n",
"    p = pulse train\n",
"    i = impulse\n",
"    d = dc, where -aN sets the value of the offset\n",
"    S = silence\n",
"    u = uniform noise\n",
"-aN = set amplitude to N (1.0) (0 to 1.0, or -96dB to -0dB)\n",
"-TN = set time in secs. to N (1.0)\n",
"-SN = duration of silence at end of tone, in secs. (0),\n",
"-H  = suppress the header when writing floatsams\n",
"flag options may be expressions\n",
"postoperators: S=time in samples, k=\"times 1000\"\n"
);
exit(1);
}

whead(sr, fc, amp, dur)
	float sr, fc, amp, dur;
{
	char buf[128];

	(void) sprintf(buf, "%f", sr);
	if (addprop(stdout, H_SRATE, buf) != 0)
		return(-1);
	(void) sprintf(buf, "%f", fc);
	if (addprop(stdout, H_FREQUENCY, buf) != 0)
		return(-1);
	(void) sprintf(buf, "%f", amp);
	if (addprop(stdout, H_AMPLITUDE, buf) != 0)
		return(-1);
	(void) sprintf(buf, "%f", dur);
	if (addprop(stdout, H_NSAMPLES, buf) != 0)
		return(-1);
	if (addprop(stdout, H_NCHANS, "1") != 0)
		return(-1);
	if (addprop(stdout, H_FORMAT, H_FLOATSAM) != 0)
		return(-1);
	return(0);
}
