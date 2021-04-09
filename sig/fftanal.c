#include<stdio.h>
#include<math.h>
#include <carl/carl.h>
#include <carl/defaults.h>

struct node {
	int flag;
	long loc;
	double val;
	};

#define UP 0
#define DN 1

struct node *np, n;

main(argc, argv)
	int  argc;
	char **argv;
{
	float input, last, srate = FDHISR, thresh = .0001;
	int cnt = 0, dir = UP, nodes = 1024, wsize = 16384, i, j, 
		floatsam = 1;
	long samp = 0;
	char ch;
	float sratio, output;

	while ((ch = crack(argc, argv, "hfFpbtn|w|T|R|", 0)) != NULL)
		{
		switch(ch)
			{
			case 'n': nodes = expr(arg_option); break;
			case 'w': wsize = expr(arg_option); break;
			case 'R': srate = expr(arg_option); break;
			case 'T': thresh = expr(arg_option); break;
			case 't': floatsam = 0; break;
			case 'h': 
			case EOF:
			default:	ffthelp();
			}
		}

	if (isatty(1) && floatsam) {
		floatsam = 0;
	}

	while (getfloat(&input) > 0) {
		if (input > last)	/* going up */
			{
			if (dir == DN)
				{	/* have just finished down slope */
				n.flag = dir;
				n.loc = samp-1;
				n.val = last;
				structsav(&np, n);
				++cnt;
				dir = UP;
				}
			}
		else			/* going down */
			{
			if (dir == UP)
				{	/* have just finished up slope */
				if (last > thresh) { 
					n.flag = dir;
					n.loc = samp-1;
					n.val = last;
					structsav(&np, n);
					++cnt;
					dir = DN;
					}
				}
			}
		last = input;
		samp++;
	}

	sratio = srate / wsize;
	for (i = j = 0; i < cnt; i++ ) {
		if (floatsam) {
			output = np[i].loc * sratio;
			putfloat(&output);
			output = np[i].val;
			putfloat(&output);
		} else {
			if (np[i].flag==UP)
				printf("%6.3f\t%6.3f\n", np[i].loc * sratio,
					20.0*log10(np[i].val));
		}
	}
	if (floatsam)
		flushfloat();
}


ffthelp() 
{
	fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s",
		"usage: fftanal [flags] < fft_power_spectrum > text\n",
		"input must be a file or pipe of floatsams\n",
		" flags:\n",
		" n = number of expected nodes (1024)\n",
		" w = window size used by the fft (16K)\n",
		" R = sample rate of original sample data (48K)\n",
		" T = minimum node threshold\n",
		" f = output binary frequency-amplitude pairs\n",
		" F = output binary amplitude only\n",
		" t = output text sample pairs (default for tty output)\n",
		" b = show long form of analysis\n");
	exit(1);
}

int bufcnt;
int buflen;

structsav(base, strct)
	struct node **base, strct;
{
	if (bufcnt >= buflen) {
		buflen += BUFSIZ;
		if (*base == NULL) {
			if ((*base = (struct node *) malloc(BUFSIZ * 
					sizeof(struct node))) == NULL)
				goto errout;
		}
		else {
			if ((*base = (struct node *) realloc(*base, 
				    buflen * sizeof(struct node))) == NULL)
				goto errout;
		}
	}

	(*base)[bufcnt].flag = strct.flag;
	(*base)[bufcnt].loc = strct.loc;
	(*base)[bufcnt].val = strct.val;
	return(++bufcnt);

errout:	fprintf(stderr, "structsav: out of memory!\n");
	return(-1);
}
