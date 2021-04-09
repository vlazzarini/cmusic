/* Cheap terminal display program - very cheap */
/*
 * this cheap hack runs faster than show.c by virtue of using the
 * curses(3) cursor optimization package to plot the display.
 * to compile:
 * cc -O fshow.c putx.c -lsf -lfrm -lm -lcurses -ltermlib
 */

#include <stdio.h>
#include <math.h>
#include <carl/frm.h>

#define OFF 8

extern int sfexprerr;
extern float sfexpr();
extern int arg_index;	/* from crack(); */
extern char *arg_option;

int stop;

main(argc,argv) int argc; char *argv[]; 
{
    char cbuf[128];
    float input, srate = 49152.0, ave = 0.0;
    int pos, i, j, n = 0, skip = 0, skipcnt = 1, avemode = 0, srm = 0;
    float min = -1,max = 1;
    char c, crack(), fill = ' ', capital = '*';
    char *file[2];

    if(isatty(0)) showhelp();

    while ((c = crack(argc, argv, "hH|C|l|u|m|R|a|e|s|", 0)) != NULL)
	    {
	    switch(c)
		    {
		    case 'l':	min = sfexpr(arg_option, 1.0); break;
		    case 'u':	max = sfexpr(arg_option, 1.0); break;
		    case 'm':	max = sfexpr(arg_option, 1.0); 
				min = -max; break;
		    case 'R':	srate = sfexpr(arg_option, 1.0); srm++; break;
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
		    default:   showhelp();
		    }
	    }

    for (i = arg_index, j = 0; i < argc; i++, j++)
	file[j] = argv[i];

    initcm();

    while ( getfloat(&input) > 0)
	{
	if (skip) 
		{ 
		if (--skipcnt) 
			{ 
			if (avemode == 1)
				ave += input * input; 
			else
			if (avemode == 0)
				ave += input; 
			continue; 
			}
		else 
			{ 
			skipcnt = skip; 
			if (avemode != -1) input = ave/skip; 
			ave = 0.0; 
			}
		}
	if (srm)
		sprintf(cbuf, "%6.3f", n/srate);
	else
		sprintf(cbuf, "%6d",n);
	c = capital;
	if (input < min) {c = '<'; input = min;}
	if (input > max) {c = '>'; input = max;}
	pos = (input-min)/(max-min) * 70.0 + 0.5;
	if (pos <= 35) 
	    {
	    if (fill != ' ') for(i = 0; i < pos; i++) 
		cbuf[i+OFF] = fill;
	    cbuf[pos+OFF] = c;
	    if (pos != 35)
		{
		cbuf[35+OFF] = '|';
	        }
	    } 
	else 
	    { 
	    if (fill != ' ') for (i=0; i<35; i++) 
		cbuf[i+OFF] = fill;
	    cbuf[35+OFF] = '|';
	    if (fill != ' ') for (i=36; i<pos; i++) 
		cbuf[i+OFF] = fill;
	    cbuf[pos+OFF] = c;
	    }
	putstr(cbuf, 80);
	n += skip == 0 ? 1 : skip;
        }
    endcm();
    }


putstr(str, len)
	char *str; int len;
{
	register int i;
	for (i = 0; i < len; i++)
	    if (str[i] != ' ' && str[i] != NULL)
		{
		putx(i, str[i]);
		str[i] = NULL;
		}
	putx(0, '\n');
	}
	



showhelp()
{
printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
"usage: show [flags] < floatsams > text\n",
" Input must be a file or pipe.\n",
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
