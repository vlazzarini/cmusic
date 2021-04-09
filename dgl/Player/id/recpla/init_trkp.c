/* %M%	%I%	(CARL)	%G%	%U% */

# include <carl/sa_mpuvar.h>
# include <sys/types.h>
# include <carl/midi.h>
# include <carl/dx7.h>
# include "medit.h"
# define  NULL 0

init_trkp(trkp, trk)
	struct mpu_trkp *trkp;
	struct mpu_trk *trk;
{
	trkp->trkp_trk = trk;
	trkp->trkp_pos = 0;
	trkp->trkp_inc = 0;
}
