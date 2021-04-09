/* %M%	%I%	(CARL)	%G%	%U% */

# include "medit.h"
# define u_char	unsigned char
# define NULL	0

setup_trk(trk, len)
	struct mpu_trk	*trk;
	register int	len;
{
	extern char 	*malloc();

	if ((trk->trk_buf = (u_char *) malloc(len)) == 0)
		Pexit(1);
	trk->trk_len = len;
	trk->trk_cnt = 0;
}
