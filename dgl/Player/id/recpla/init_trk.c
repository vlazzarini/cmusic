/* %M%	%I%	(CARL)	%G%	%U% */

/* %M%	%I%	(CARL)	%G%	%U% */

# include <carl/sa_mpuvar.h>
# include "medit.h"

init_trk(mt)
	struct mpu_trk	mt[];
{
	register int	i;

	for (i = 0; i < P_TRK; i++) {
		mt[i].trk_buf = (unsigned char) 0;
		mt[i].trk_len = -1;
		mt[i].trk_cnt = 0;
		mt[i].trk_pos = 0;
		mt[i].trk_mlen = 0;
	}
}
