#define FBUFSIZE 1024
struct sndesc{
	int	nc;
	int	fs;
	float	sr;
	char	*fullname;
	float	*samps;
	int	actsamp;
};
