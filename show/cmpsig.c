#include <stdio.h>
#include <math.h>
#include <carl/carl.h>
#include <carl/defaults.h>
#include "plotline.c"

#define NFILES 16

extern int sfexprerr;
extern float sfexpr();
extern int arg_index;	/* from crack(); */
extern char *arg_option;

char *file[NFILES];
FILE *fid[NFILES]; 
int nfiles, nchans = 1, channels, usestdin = 0, each = 0;
float inputs[NFILES], ave[NFILES];
int n[NFILES], pos[NFILES], done[NFILES];
float *buf[NFILES];

main(argc,argv) int argc; char *argv[]; 
{
    float input, srate = FDHISR;
    long pos, i, j, n = 0, skip = 0, skipcnt = 1, avemode = 0, srm = 0;
    float min = -1,max = 1;
    char c, crack(), fill = ' ', capital = 'A';
    int textmode = 0;

    while ((c = crack(argc, argv, "thH|c|C|l|u|m|R|a|e|s|E|", 0)) != NULL)
	    {
	    switch(c)
		    {
		    case 't':	textmode++;
		    case 'l':	min = sfexpr(arg_option, 1.0); break;
		    case 'u':	max = sfexpr(arg_option, 1.0); break;
		    case 'm':	max = sfexpr(arg_option, 1.0); 
				min = -max; break;
		    case 'R':	srate = sfexpr(arg_option, 1.0); 
				if (srate == 0) srate = 48*1024;
				srm++; break;
		    case 'a':	skipcnt = skip = sfexpr(arg_option, srate); 
				avemode = 0; break;
		    case 'e':	skipcnt = skip = sfexpr(arg_option, srate); 
				avemode = 1; break;
		    case 's':	skipcnt = skip = sfexpr(arg_option, srate); 
				avemode = -1; break;
		    case 'H':	fill = *arg_option; 
			        if (fill == NULL) fill = '-'; 
			        break;
		    case 'C':	capital = *arg_option; 
			        if (capital == NULL) capital = '*'; 
			        break;
		    case 'E':	each = sfexpr(arg_option, 1.0);
				break;
		    case 'c':	nchans = sfexpr(arg_option, 1.0); break;
		    case 'h': 
		    default:   cmpsighelp();
		    }
	    }

    for (i = arg_index, j = 0; i < argc; i++, j++)
	{
	if (j > NFILES) 
		{ 
		fprintf(stderr, "cmpsig: too many files (lim. 16)\n"); 
		exit(1);
		}
	file[j] = argv[i];
	}

    for (i = 0, nfiles = j; i < nfiles; i++) {
	if ((fid[i] = fopen(file[i], "r")) == NULL) {
		printf("cmpsig: %s not found.\n", file[i]);
		exit(1);
	}
	buf[i] = (float *) calloc(sizeof(float) * BUFSIZ, 1);
    }

    if (nfiles == 0) {
	usestdin++;
	channels = nchans;
    } else if (each != 0)
	channels = nchans;
    else
	channels = nfiles;

    while ( get_inputs()  > 0)
	{
	if (skip) 
		{ 
		if (--skipcnt) 
			{ 
			if (avemode == 1)
				for (i = 0; i < channels; i++)
					ave[i] += inputs[i] * inputs[i];
			else
			if (avemode == 0)
				for (i = 0; i < channels; i++)
					ave[i] += inputs[i];
			continue; 
			}
		else 
			{ 
			skipcnt = skip; 
			if (avemode != -1) 
				for (i = 0; i < channels; i++)
					{
					inputs[i] = ave[i] / skip;
					ave[i] = 0.0; 
					}
			}
		}

	if (!textmode) {
		for (i = 0; i < channels; i++) {
			if (done[i] > 0)
				addplot(inputs[i], capital+i, fill, min, max);
		}
		plotit(n, srate, srm, fill);
	} else
		ptext(n);
	n++;
        }
}


cmpsighelp()
{
printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
"usage: cmpsig [flags] file1 file2 file3 ... file16 > text\n",
"or:    cmpsig [flags] < floatsams > text\n",
" flags:\n",
" -t  = set text mode output instead of CRT graphics\n",
" -lN = set lower bound of display to N (-1)\n",
" -uN = set upper bound of display to N (+1)\n",
" -mN = set upper and lower bounds to + and - N ([-1,+1])\n",
" -RN = show time instead of sample number using sampling rate N\n",
" -cN = set number of channels if reading stdin\n",
" -aN = display average of N seconds of samples.\n",
" -eN = display mean squared energy of N seconds of samples\n",
" -sN = skip output by N seconds worth of samples of input\n",
" -Cc = show sample value as character c ('*')\n",
" -Hc = histogram mode using character c ('-')\n",
" -EN = each file has N channels (set -c to total channels)\n",
"All durations are in seconds.  Use postop 'S' for sample times.\n",
"Arguments may be expressions.\n"
);
exit(1);
}

get_inputs()
{
	register int i, more = 0;

	if (usestdin) {
		for (i = 0; i < channels; i++) {
			done[i] = getfloat(&inputs[i]);
			if (done[i] < 0) {
				fprintf(stderr, "cmpsig: ran out of stdin\n");
				exit(1);
			}
			more += done[i];
		}
	} else {
		if (each) {
			int j = each, k = 0;
			for (i = more = 0; i < nchans; i++) {
				done[i] = fgetfloat(&inputs[i], fid[k]);
				if (done[i] < 0) {
					fprintf(stderr, 
						"cmpsig: error reading %s\n", 
						file[i]);
					exit(1);
				}
				more += done[i];
				if (j-- == 0) {
					k++;
					j = each;
				}
			}
		} else {
			for (i = more = 0; i < channels; i++) {
				done[i] = fgetfloat(&inputs[i], fid[i]);
				if (done[i] < 0) {
					fprintf(stderr, 
						"cmpsig: error reading %s\n", 
						file[i]);
					exit(1);
				}
				more += done[i];
			}
		}
	}
	return(more);
}

ptext(n)
	long n;
{
	register int i;

	printf("%d:\t", n);
	for (i = 0; i < nfiles; i++) {
		if (done[i])
			printf("%f\t", inputs[i]);
		else
			printf("%f\t", 0);
	}
	printf("\n");
}
