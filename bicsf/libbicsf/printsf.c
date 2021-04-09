#include "sfheader.h"
#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
static SFCODE	ampcode = {
	SF_MAXAMP,
	sizeof(SFMAXAMP) + sizeof(SFCODE)
}; 
printsf(sfh)
SFHEADER *sfh;
{
	SFMAXAMP sfm;
	SFCODE *sizer;
	SFCOMMENT sfcm;
	char *cp,*getsfcode();
	char date[26];
	int i;

	printf("sr: %5.0f nchans: %d class: %d\n",
		sfsrate(sfh),sfchans(sfh),sfclass(sfh));
	cp = getsfcode(sfh,SF_MAXAMP);
	if(cp != NULL)
		bcopy(cp + sizeof(SFCODE), (char *) &sfm, sizeof(SFMAXAMP));
	if(cp != NULL) {
		for(i=0; i<sfchans(sfh); i++)
		printf("channel %d: maxamp: %e loc: %d\n",
		i,sfmaxamp(&sfm,i),sfmaxamploc(&sfm,i));
		cp = getsfcode(sfh,SF_COMMENT);
		if(sfmaxamptime(&sfm)) {
			strcpy(date,ctime(&sfmaxamptime(&sfm))); 
			printf("date=%s\n",date);
		}
	}
	printf("\n");
	if(cp != NULL) {
		sizer = (SFCODE *) cp;
		bcopy(cp + sizeof(SFCODE) , (char *) &sfcm, sizer->bsize);
		printf("Comment on soundfile: \n%s\n",&sfcomm(&sfcm,0));
	}
}
