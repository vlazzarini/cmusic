/* cc equtemp.c -lm */

#include<stdio.h>
#include<math.h>
#include <stdlib.h>
char *pitch[] = {"A","Bb","B","C","CS","D","Eb","E","F","FS","G","GS"}; 

int flag;

main(argc, argv)
	char **argv;
{
	register int i, cnt = 0;
	register char *c;
	register float frq, tfrq;
	if (argc >= 2)
	    {
	    while (--argc)
		{
		frq = atof(argv[++cnt]); 
		for (i = 0; i < 88; i++)	
			{
			tfrq = (440.0/16.0)*pow(2.0, i/12.0);
			if (tfrq >= frq)
				{
				register float lfrq, diff, cdiff;
				lfrq = (440.0/16.0)*pow(2.0, (i-1)/12.0);
				cdiff = tfrq - lfrq;
				diff = (frq-lfrq)/cdiff; 
				if (diff < .5 ) i--; 
				c = *(pitch+(i % 12));
				printf("%d:\t%s%d\t%f\t", i+1, c, (i+9)/12, 
					(440.0/16.0)*pow(2.0, i/12.0));
				if (diff == 0 || diff == 1.0)
					printf("\n");
				else
					{
					printf("%f is ", frq);
					if (diff < .5) 
						printf("%f cents sharp\n", 
							diff*100.0);
					else 
						printf("%f cents flat\n",
						    (1.0-diff)*100.0);
					}
				break;
				}
			}
		}
		exit(0);
	    }
	else
	    for (i = 0; i < 88; i++)
		    {
		    c = *(pitch+(i % 12));
		    printf("%d:\t%s%d\t%f\n", i+1, c, (i+9)/12, 
			    (440.0/16.0)*pow(2.0, i/12.0));
		    }
	    }
