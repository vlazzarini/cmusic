#include <carl/libran.h>

/*
 * corrand - variable correlated number generator.
 * 
 * This routine produces noise that goes from white to increasingly more
 * correlated (to a dc constant) as cor_factor goes from 0 to 1.  Values
 * are produced in the range of lb to ub.  Informal tests have shown that
 * values of cor_factor=.96 produce roughly the same spectrogram as 1/f
 * noise (drops 6dB per octave when piped to spect).  Values of cor_factor
 * much greater than that roll off much more quickly.  Values in excess of
 * .99 tend to never get out to the extremes of the boundaries except in
 * very large samples.  For cor_factor=1.0, the output will continue to be
 * whatever was the last contents of save.
 * It is interesting to compare true 1/f noise to the result of this
 * routine.
 */

double cor_factor;

double corrand(lb, ub)
	double lb, ub;
{
	static double save;
	double range, wlb, wub;

	range = (ub-lb) - (ub-lb) * cor_factor;
	wlb = save - range;
	if (wlb < lb) wlb = lb;
	wub = save + range;
	if (wub > ub) wub = ub;
	save = frand(wlb, wub);
	return(save);
	}

/*
 * #include<stdio.h>
 * #include<math.h>
 * 
 * main(argc, argv)
 * 	char **argv;
 * {
 * 	int otty = isatty(1), n;
 * 	float input, lb, ub;
 * 	n = atoi(argv[1]);
 * 	cor_factor = atof(argv[2]);
 * 	lb = atof(argv[3]);
 * 	ub = atof(argv[4]);
 * 	while (n--)
 * 		{
 * 		input = corrand(lb, ub);
 * 		if (otty) printf("%f\n", input);
 * 		else putfloat(&input);
 * 		}
 * 	flushfloat();
 * 	}
 * 
 */
