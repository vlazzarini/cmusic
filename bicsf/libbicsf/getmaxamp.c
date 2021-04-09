# include <stdio.h>

#ifndef LINT
static char SccsId[] = "@(#)getmaxamp.c	1.2	4/27/86	IRCAM";
#endif

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/file.h>
# include <sfheader.h>

# define BAD goto bad

/* This saves half the CPU time for the job just in the use of the
   soubroutine call as abs() and fabs() are assembler routines! */

# define abs(x) ((x) < 0 ? -(x) : (x))

/* This routine will find the peak absolute value per channel 
   and the sample location of an existing soundfile window 
   and if the entire file was scanned and update is turned on and
   the user has permission 
   that information plus a timetag will be put in the header */

static float maxamp[SF_MAXCHAN];
static long bytes,totalbytes, maxloc[SF_MAXCHAN];

static SFMAXAMP sfm;

static SFCODE	ampcode = {
	SF_MAXAMP,
	sizeof(SFMAXAMP) + sizeof(SFCODE)
}; 

SFMAXAMP *getmaxamp(fd,update,startsampf,dursampf)
	long startsampf, dursampf;
{
	int readbyte;
	int i;
	SFHEADER hd;
	char *buffer,*malloc(),*getsfname();
	struct stat st;
	long oldloc,samples;

	/* Get memory for buffers */

	buffer = malloc(SF_BUFSIZE);
	if(!buffer) 
		return(NULL);;

	if(fstat(fd,&st) == -1)
		return(NULL);

	oldloc = lseek(fd,0,1);
	lseek(fd,0,0); /* To reread header */

	if(rheader(fd,&hd)) 
		BAD;

	totalbytes = bytes = dursampf * sfchans(&hd) * sfclass(&hd); 
	startsampf = startsampf * sfchans(&hd) * sfclass(&hd);
	if (sfbsize(&st) < startsampf + totalbytes)
		return(NULL);

	bzero(&sfm,sizeof(sfm));
	bzero(maxamp,sizeof(maxamp));
	bzero(maxloc,sizeof(maxloc));

	sflseek(fd,startsampf,0);
	while(bytes > 0) { /* For all of the samples */
		if((readbyte = read(fd,buffer,SF_BUFSIZE)) < 0) 
			BAD;
		samples = readbyte/sfclass(&hd);
		if(sfclass(&hd) == SF_FLOAT) 
			ffindamp(buffer,samples,sfchans(&hd));
		else
			sfindamp(buffer,samples,sfchans(&hd));
		bytes -= readbyte;
	}
	for(i = 0; i < sfchans(&hd); i++) {
		sfmaxamploc(&sfm,i) = maxloc[i]/sfclass(&hd)/sfchans(&hd);
		sfmaxamp(&sfm,i) = maxamp[i];
	}
	if(update && (startsampf + totalbytes == sfbsize(&st))) {
		/* Update the header */
		sfmaxamptime(&sfm) = time(0);
		if(putsfcode(&hd,&sfm,&ampcode))
			return(NULL);
		lseek(fd,0,0); 
		if(wheader(fd,&hd))
			BAD;
	}
	lseek(fd,oldloc,0);
	return(&sfm);

bad:	lseek(fd,oldloc,0);
	return(NULL);
}

ffindamp(buf,samples,chans)
	char *buf;
	long samples;
{
	register float *fbuffer = (float *) buf;
	register float val;
	register int i;

	while(samples > 0) {
		for(i = 0; i < chans; i++) {
			val = abs(*fbuffer);
			if(val > maxamp[i]) {
				maxamp[i] = val;
				maxloc[i] = totalbytes - bytes +
					(char *) fbuffer - buf;
			}
			fbuffer++;
		}
		samples -= chans;
	}

}

sfindamp(buf,samples,chans)
	char *buf;
	register long samples;
{
	register short *sbuffer = (short *) buf;
	register float val;
	register int i;

	while(samples > 0) {
		for(i = 0; i < chans; i++) {
			val = abs(*sbuffer);
			if(val > maxamp[i]) {
				maxamp[i] = val;
				maxloc[i] = totalbytes - bytes +
					(char *) sbuffer - buf;
			}
			sbuffer++;
		}
		samples -= chans;
	}
}
