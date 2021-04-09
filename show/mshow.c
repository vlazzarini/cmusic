#include <stdio.h>
#include <math.h>
#include <carl/carl.h>
#include <carl/defaults.h>

#define NCHANS 16

extern int sfexprerr;
extern float sfexpr();
extern int arg_index;	/* from crack(); */
extern char *arg_option;

char *file[NCHANS];
FILE *fid[NCHANS]; 
int nfiles;
float inputs[NCHANS], ave[NCHANS];
int done[NCHANS], nchans = 1;

main(argc,argv) 
    int argc; char *argv[]; 
{
    float input, srate = FDEFSR;
    int pos, i, j, n = 0, skip = 0, skipcnt = 1, avemode = 0, srm = 0;
    float min = -1,max = 1;
    char c, crack(), fill = ' ', capital = 'A';

    while ((c = crack(argc, argv, "hH|C|l|u|m|R|a|e|s|c|", 0)) != NULL)
	    {
	    switch(c)
		    {
		    case 'l':	min = sfexpr(arg_option, 1.0); break;
		    case 'u':	max = sfexpr(arg_option, 1.0); break;
		    case 'm':	max = sfexpr(arg_option, srate); 
				min = -max; break;
		    case 'R':	srate = sfexpr(arg_option, 1.0); 
				if (srate == 0) srate = 48*1024;
				srm++; break;
		    case 'c':	nchans = sfexpr(arg_option, 1.0); break;
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
		    case 'h': 
		    default:   cmpsighelp();
		    }
	    }

    for (i = arg_index, nfiles = 0; i < argc; i++, nfiles++)
	{
	if (nfiles > NCHANS) 
		{ 
		fprintf(stderr, "cmpsig: too many files (lim. 8)\n"); 
		exit(1);
		}
	file[nfiles] = argv[i];
	}

    for (i = 0; i < nfiles; i++)
	{
	if ((fid[i] = fopen(file[i], "r")) == NULL)
		{
		printf("cmpsig: %s not found.\n", file[i]);
		exit(1);
		}
	}

    while ( get_inputs()  > 0)
	{
	if (skip) 
		{ 
		if (--skipcnt) 
			{ 
			if (avemode == 1)
/*				ave += input * input; */
				for (i = 0; i < nfiles; i++)
					ave[i] += inputs[i] * inputs[i];
			else
			if (avemode == 0)
/*				ave += input; */
				for (i = 0; i < nfiles; i++)
					ave[i] += inputs[i];
			continue; 
			}
		else 
			{ 
			skipcnt = skip; 
			if (avemode != -1) 
/*				input = ave/skip; */
				for (i = 0; i < nfiles; i++)
					{
					inputs[i] = ave[i] / skip;
					ave[i] = 0.0; 
					}
			}
		}

		if (nfiles == 0) {
			for (i = 0; i < nchans; i++)
				if (done[i] > 0)
					addplot(inputs[i], capital+i, 
						fill, min, max);
		} else {
			for (i = 0; i < nfiles; i++)
				if (done[i] > 0)
					addplot(inputs[i], capital+i, 
						fill, min, max);
		}

		plotit(n++, srate, srm, fill);
	}
}


cmpsighelp()
{
printf("%s%s%s%s%s%s%s%s%s%s%s%s%s",
"usage: cmpsig [flags] file1 file2 file3 ... file16 > text\n",
" flags:\n",
" -lN = set lower bound of display to N (-1)\n",
" -uN = set upper bound of display to N (+1)\n",
" -mN = set upper and lower bounds to + and - N ([-1,+1])\n",
" -RN = show time instead of sample number using sampling rate N\n",
" -aN = display average of N seconds of samples.\n",
" -eN = display mean squared energy of N seconds of samples\n",
" -sN = skip output by N seconds worth of samples of input\n",
" -Cc = show sample value as character c ('*')\n",
" -Hc = histogram mode using character c ('-')\n",
"All durations are in seconds.  Use postop 'S' for sample times.\n",
"Arguments may be expressions.\n"
);
exit(1);
}

get_inputs()
{
	register int i, more;

	if (nfiles == 0) {
		for (i = 0; i < nchans; i++) {
			done[i] = getfloat(&inputs[i]);
			if (done[i] < 0) {
				fprintf(stderr, "cmpsig: ran out of stdin\n");
				exit(1);
			more += done[i];
			}
		}
	} else {
		for (i = more = 0; i < nfiles; i++) {
			done[i] = fgetfloat(&inputs[i], fid[i]);
			if (done[i] < 0) 
				{
				fprintf(stderr, 
					"cmpsig: error reading %s\n", file[i]);
				exit(1);
				}
			more += done[i];
		} 
	}
	return(more);
}
