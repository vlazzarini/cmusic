#include <sys/types.h>
#include <sys/stat.h>
#include <sfheader.h>
#include <stdio.h>
#define IRCAM_SFS
#include <sndio.h>
#include <snd.h>

char	*getsfname();
struct	sndesc	*opensf(name)
	char	*name;
{
	int sfd;
	SFHEADER sfh;
	struct stat sfstat;
	struct sndesc	*snd;
	int result;
	char	*fullname;

	snd = (struct sndesc *)malloc(sizeof(struct sndesc));

	fullname = getsfname(name);
	snd->fullname=(char *)malloc(strlen(fullname)+1);
	strcpy(snd->fullname,fullname);

	result=0;
	readopensf(snd->fullname,sfd,sfh,sfstat,"opensf",result)
	if (result==0) {
		snd->nc=sfchans(&sfh);
		snd->fs=sfbsize(&sfstat)/sfclass(&sfh);
		snd->sr=sfsrate(&sfh);
		close(sfd);
		snd->actsamp= -1;
		return(snd);
	}
	else
		return((struct sndesc *) NULL);
}
closesf(sndptr)
	struct	sndesc *sndptr;
{
	free(sndptr->fullname);
	free(sndptr->samps);
	free(sndptr);
}
