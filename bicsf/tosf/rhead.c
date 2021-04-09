#include <stdio.h>
#include <sndio.h>
#include <carl/defaults.h>
#include <carl/carl.h>
#include <carl/procom.h>
#include <sfheader.h>
#include <string.h>

rhead(sfh, ipak, iop)
	SFHEADER *sfh;
	char *ipak;
	FILE *iop;
{

	char *pval;


	if (getheader(iop) == NULL)
		return(0);

	if ((pval = getprop(iop, H_FORMAT)) != NULL){
		if (!strcmp(pval, H_FLOATSAM))
			*ipak = PMFLOAT;
		else
			*ipak = PM16BIT;
	}

	if ((pval = getprop(iop, H_NCHANS)) != NULL)
		sfchans(sfh) = atoi(pval);

	if ((pval = getprop(iop, H_SRATE)) != NULL)
		sfsrate(sfh) = atof(pval);

	if ((pval = getprop(iop, H_SNDOUT_FORMAT)) != NULL)
		if (!strcmp(pval, H_FLOATSAM))
			sfclass(sfh) = SF_FLOAT;

	return(0);
}
