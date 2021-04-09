#include <sys/types.h>
#include <sys/stat.h>
#include <sfheader.h>
#include <stdio.h>
# define IRCAM_SFS
#include <sndio.h>
#include <snd.h> 

int sferror;
float	fsndi(sndptr,sampnum)
	struct	sndesc	*sndptr;
	long	sampnum;
{

	int	begbuf,endbuf,deplas;
	
	if((sampnum<0)||(sampnum>=sndptr->fs)){
		sferror=1;
		return(0.0);
	}
	if (sndptr->actsamp < 0)
		sndptr->samps=(float *)malloc(FBUFSIZE*sizeof(float));

	deplas=sampnum%FBUFSIZE;
	begbuf=sampnum-deplas;
	endbuf=begbuf+FBUFSIZE;
	endbuf=(endbuf>sndptr->fs)?sndptr->fs:endbuf;
	if(begbuf!=sndptr->actsamp){
	    if(getflbuf(sndptr->samps,begbuf,endbuf-begbuf,sndptr->fullname)<0){
			sferror=1;
			return(0.0);
		}
		sndptr->actsamp=begbuf;
	}
	return(sndptr->samps[deplas]);
}
