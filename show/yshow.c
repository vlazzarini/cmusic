/* Cheap terminal display program - very cheap */
/* compile as 
 * % cc yshow.c puty.c putx.c -lcurses -ltermlib -lsf -lfrm -lm
 */

#include <stdio.h>
#include <math.h>
#include <carl/frm.h>

#define OFF 0
#define MID 11
#define ABCISSA '-'
#define MAX 23.0

extern int sfexprerr;
extern float sfexpr();
extern int arg_index;	/* from crack(); */
extern char *arg_option;

int stop;
int wait;

main(argc,argv) int argc; char *argv[]; 
{
    char cbuf[128];
    float input, srate = 49152.0, ave = 0.0, sfexpr();
    int pos, i, j, n = 0, skip = 0, skipcnt = 1, avemode = 0, srm = 0;
    float min = -1,max = 1;
    char c, crack(), fill = ' ', capital = '*';
    char *file[2];
    int scrlcnt=15;


    if(isatty(0)) showhelp();

    while ((c = crack(argc, argv, "hH|C|l|u|m|R|a|e|s|S|w", 0)) != NULL)
	    {
	    switch(c)
		    {
		    case 'l':	min = sfexpr(arg_option, 1.0); break;
		    case 'u':	max = sfexpr(arg_option, 1.0); break;
		    case 'm':	max = sfexpr(arg_option, srate); 
				min = -max; break;
		    case 'R':	srate = sfexpr(arg_option, 1.0); srm++; break;
		    case 'a':	skipcnt = skip = sfexpr(arg_option, srate); 
				avemode = 0; break;
		    case 'e':	skipcnt = skip = sfexpr(arg_option, srate); 
				avemode = 1; break;
		    case 's':	skipcnt = skip = sfexpr(arg_option, srate); 
				avemode = -1; break;
		    case 'H':	fill = *arg_option; 
				if (fill == NULL) fill = '|'; 
				break;
		    case 'C':	capital = *arg_option; 
				if (capital == NULL) capital = '*'; 
				break;
		    case 'S':	scrlcnt = sfexpr(arg_option, 1.0); break;
		    case 'w':	wait++; break;
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
/*
 * 	if (srm)
 * 		sprintf(cbuf, "%6.3f", n/srate);
 * 	else
 * 		sprintf(cbuf, "%6d",n);
 */
	c = capital;
	if (input < min) {c = '<'; input = min;}
	if (input > max) {c = '>'; input = max;}
	pos = (input-min)/(max-min) * MAX + 0.5;
	if (pos <= MID) 
	    {
	    if (fill != ' ') for(i = 0; i < pos; i++) 
		cbuf[i+OFF] = fill;
	    cbuf[pos+OFF] = c;
	    if (pos != MID) cbuf[MID+OFF] = ABCISSA;
	    } 
	else 
	    { 
	    if (fill != ' ') for (i=0; i<MID; i++) 
		cbuf[i+OFF] = fill;
	    cbuf[MID+OFF] = ABCISSA;
	    if (fill != ' ') for (i=MID+1; i<pos; i++) 
		cbuf[i+OFF] = fill;
	    cbuf[pos+OFF] = c;
	    }
	putstr(cbuf, 24, scrlcnt);
	n += skip == 0 ? 1 : skip;
        }
    endcm();
    }


putstr(str, len, scrlcnt)
	char *str; int len; int scrlcnt;
{
	register int i;
	for (i = 0; i < len; i++)
	    if (str[i] != ' ' && str[i] != NULL)
		{
		puty(len - i - 1, str[i], MID+1, ABCISSA, scrlcnt);
		str[i] = NULL;
		}
	puty(0, '\n', MID+1, ABCISSA, scrlcnt);
	if (wait) rtn();
	}

rtn()
    {
    char ans[80];
    FILE *fopen(), *fp;
    fp = fopen("/dev/tty", "r");
    fgets(ans, 80, fp);
    fclose(fp);
    }

	
showhelp()
{
printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
"usage: show [flags] < floatsams > text\n",
" Input must be a file or pipe.\n",
" flags:\n",
" -SN = set # samples to jump display over when scrolling\n",
" -lN = set lower bound of display to N (-1)\n",
" -uN = set upper bound of display to N (+1)\n",
" -mN = set upper and lower bounds to + and - N ([-1,+1])\n",
" -aN = display average of N seconds of samples.\n",
" -eN = display mean squared energy of N seconds of samples\n",
" -sN = skip output by N seconds worth of samples of input\n",
" -Cc = show sample value as character c ('*')\n",
" -Hc = histogram mode using character c ('|')\n",
"All durations are in seconds.  Use postop 'S' for sample times.\n",
"Arguments may be expressions.\n"
);
exit(1);
}
