#include <stdio.h>
#include <carl/carl.h>
#include <carl/defaults.h>

main(argc, argv)
	int argc; char **argv;
{
    long int i;
    int omode = isatty(1);
    char ch;
    float input;
    float srate = FDEFSR;

    if (isatty(0)) nonzerohelp(0);

    while ((ch = crack(argc, argv, "R|", 0)) != NULL) 
	{
	switch (ch) 
		{
		case 'R': srate = sfexpr(arg_option, 1.0); break;
		default: nonzerohelp(1);
		}
	}

    while (getfloat(&input) > 0)
	{
	if (input != 0.0)
	    if (omode)
		printf("%ds\t(%fS)=\t%f\n", i, i/srate, input);
	    else
		putfloat(&input);
	i++;
	}
    if (!omode) flushfloat();
    }

nonzerohelp(x)
{
fprintf(stderr, "%s%s%s%s%s%s%s",
"nonzero [flag] < floatsams > output\n",
" flag:\n",
" -RN\t set sample rate to N\n",
"If output is a file/pipe, non-zero floatsams are written.\n",
"If output is a terminal, sample #, time re. ",
DHISR,
" srate, and value are given.\n");
exit(x);
}
