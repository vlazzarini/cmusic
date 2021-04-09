/* snd.h	1.1	(CMIL)	8/2/88	16:32:51 */

#define FBUFSIZE 1024
struct sndesc{
	int	nc;
	int	fs;
	float	sr;
	char	*fullname;
	float	*samps;
	int	actsamp;
};
