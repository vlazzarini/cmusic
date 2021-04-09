/* %M%	%I%	(CARL)	%G%	%U% */

# include 	<carl/sa_mpuvar.h>
# include 	<sys/types.h>
# include 	<carl/midi.h>
# include 	<carl/dx7.h>
# include 	"medit.h"
# define 	NULL 0

dump_trk(trk)
	struct mpu_trk	*trk;
{
	register int		i, 
				cnt;
	register unsigned char	*buf;

	if (trk->trk_buf == NULL) {
		fprintf(0, "dump_trk: no buffer\n");
		return;
	}

	buf = trk->trk_buf;
	cnt = trk->trk_cnt;

	for (i = 0; i < cnt; i++)
		fprintf(0, "%x ", buf[i] & 0xff);

	fprintf(0, "\n");
}
