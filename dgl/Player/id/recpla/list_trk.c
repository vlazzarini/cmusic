/* %M%	%I%	(CARL)	%G%	%U% */

# include <carl/sa_mpuvar.h>
# include <sys/types.h>
# include <carl/midi.h>
# include <carl/dx7.h>
# include "medit.h"
# define  NULL 0

list_trk(trk) 
	struct mpu_trk	*trk;
{
	extern	int 	printit;
	struct mpu_trkp	trkp;
	register int	len, pos, i;
	register u_char	*buf;

	if (trk->trk_buf == NULL) {
		fprintf(0, "list_trk: no buf\n");
		Pexit(1);
	}
	init_trkp(&trkp, trk);
	buf = trk->trk_buf;
	while ((len = next_cmd(&trkp)) >  0) {
		if (printit == 2)
			fprintf(0, "trkp_pos=%d, trkp_inc=%d\n",
				trkp.trkp_pos, trkp.trkp_inc);
		pos = trkp.trkp_pos;
		for (i = 0; i < len; i++)
			fprintf(0, "%x ", buf[pos+i] & 0xff);
		fprintf(0, "\n");
	}
}
