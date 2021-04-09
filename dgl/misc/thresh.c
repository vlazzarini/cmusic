#include <stdio.h>
#include <carl/defaults.h>
#include <carl/carl.h>

extern int arg_index;		/* from crack() in -lsf */
extern char *arg_option;
extern float expr();		/* from -lfrm */

main(argc, argv)
	int argc; char **argv;
{
    int cnt = 0, omode = isatty(1);
    char ch;
    float input, srate = FDEFSR;
    float ub = 1.0, lb = -1.0;
    if (isatty(0)) threshhelp();

    while ((ch = crack(argc, argv, "R|", 1)) != NULL) {
	if (ch == 'R') { srate = expr(arg_option); break; }
	}

    arg_index = 0;
    while ((ch = crack(argc, argv, "l|u|m|R|h", 0)) != NULL) 
	{
	switch (ch) 
		{
		case 'l': lb = expr(arg_option); break;
		case 'u': ub = expr(arg_option); break;
		case 'm': ub = expr(arg_option); lb = -ub; break;
		case 'R': break;
		case 'h': threshhelp(0);
		default: threshhelp(1);
		}
	}

    while (getfloat(&input) > 0) 
	{
	if (input > ub || input < lb)
	    if (omode)
		printf("%d\t(%f)=\t%f\n", cnt, cnt/srate, input);
	    else
		putfloat(&input);
	cnt++;
	}
    }

threshhelp(x)
{
fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s%s%s",
"thresh [flags] < floatsams\n",
" flags:\n",
" -lN\tset lower bound to N\n",
" -uN\tset upper bound to N\n",
" -mN\tset both upper and lower bound to N\n",
" -RN\tset sample rate to N\n",
"Input must be file or pipe.\n",
"If output is file/pipe, thresh writes floatsams exceeding -l and -u values.\n",
"If output is terminal, writes sample #, (time re. ",
DEFSR,
" Hz), sample value.\n",
"Arguments may be expressions.\n");
exit(x);
}
