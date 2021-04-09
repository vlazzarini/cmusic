#include <sys/types.h>
#include <sys/stat.h>
#include <sfheader.h>
#include <stdio.h>

getflbuf(buf,beg,dur,name)
	float	*buf;
	char	*name;
{
	SFHEADER sfh;
	struct stat sfstat;
	int result=0,i,sfd;
	short	*sh;

	result=0;
	readopensf(name,sfd,sfh,sfstat,"getflbuf",result)
	if (result==0) {
		if(sfbsize(&sfstat)/sfclass(&sfh) < beg+dur){
			close(sfd);
			return(-1);
		}
		if(sfclass(&sfh)==SF_SHORT){
			if((sh=(short *) malloc(dur*sizeof(short)))<=0){
				close(sfd);
				return(-1);
			}
			if(sflseek(sfd,beg*sizeof(short),0)<=0){
				close(sfd);
				return(-1);
			}
			if(read(sfd,sh,dur*sizeof(short))!=dur*sizeof(short)){
				close(sfd);
				return(-1);
			}
			for(i=0;i<dur;i++)
				buf[i]=(float)sh[i]/32767.0;
			close(sfd);
			free(sh);
			return(dur);
		}
		else{
			if(sflseek(sfd,beg*sizeof(float),0)<=0){
				close(sfd);
				return(-1);
			}
			if(read(sfd,buf,dur*sizeof(float))!=dur*sizeof(float)){
				close(sfd);
				return(-1);
			}
			close(sfd);
			return(dur);
		}
	}
	close(sfd);
	return(-1);
}
