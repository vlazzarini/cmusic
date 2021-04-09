#include<stdio.h>
#define TAP 't'
#define CMB 'c'
#define APS 'a'

/*
 * 
 * line format: <spec_char> <coeff> <length> 
 * 	spec_char :== t | c | a  (tap, comb, or allpass respectively)
 */

extern float *apg1, *apdlys, *cmbg1, *cmbdlys, *tdmg, *tdmt;


/*
 * main(argc, argv)
 * 	char **argv;
 * {
 * 
 * 	register int i;
 * 	if (revconf(argv[1]))
 * 		{
 * 		fprintf(stderr, "lprev: configuration error\n");;
 * 		exit(-1);
 * 		}
 * 	for (i = 0; i < 32; i++)
 * 		printf("t %f %f\tc %f %f\ta%f %f\n",
 * 			tdmg[i], tdmt[i], cmbg1[i], cmbdlys[i],
 * 			apg1[i], apdlys[i]);
 * 	}
 */


revconf(name)
	char *name;
{
	register int i, j, k;
	int ndmtaps=1, ncmbs=1, nap=1;
	char c, buf[BUFSIZ], *bufp1 = buf+1, *bufp2 = buf+2;
	FILE *fp, *fopen();
	if ((fp = fopen(name, "r")) == NULL) return(-1);

	while (fgets(buf, BUFSIZ, fp) != NULL) 
		{
		*bufp1 = NULL;
		switch (*buf)
			{
			case TAP: ndmtaps++; break;
			case CMB: ncmbs++; break;
			case APS: nap++; break;
			default: break;
			}
		}

	tdmg = (float *) malloc(sizeof(float)*ndmtaps);
	tdmt = (float *) malloc(sizeof(float)*ndmtaps);
	cmbg1 = (float *) malloc(sizeof(float)*ncmbs);
	cmbdlys = (float *) malloc(sizeof(float)*ncmbs);
	apg1 = (float *) malloc(sizeof(float)*nap);
	apdlys = (float *) malloc(sizeof(float)*nap);

	fclose(fp); 
	if ((fp = fopen(name, "r")) == NULL) return(-1); 

	while (fgets(buf, BUFSIZ, fp) != NULL) 
		{
		*bufp1 = NULL;
		switch (*buf)
			{
			case TAP: if ((i=addtap(bufp2)) < 0) return(-1); break;
			case CMB: if ((j=addcmb(bufp2)) < 0) return(-1); break;
			case APS: if ((k=addaps(bufp2)) < 0) return(-1);
			default: break;
			}
		}
	tdmt[i] = cmbdlys[j] = apdlys[k] = -1.0; /* terminate list */
	fclose(fp);
	}


addtap(str)
	char *str;
{
	static int i;
	if (sscanf(str, "%f%f", &tdmg[i], &tdmt[i]) < 2) return(-1);
	return(++i);
	}

addcmb(str)
	char *str;
{
	static int i;
	if (sscanf(str, "%f%f", &cmbg1[i], &cmbdlys[i]) < 2) return(-1);
	return(++i);
	}

addaps(str)
	char *str;
{
	static int i;
	if (sscanf(str, "%f%f", &apg1[i], &apdlys[i]) < 2) return(-1);
	return(++i);
	}
