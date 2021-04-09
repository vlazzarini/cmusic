/* ptm.c	2.1	(CARL)	10/26/85	23:43:20 */

# include <sys/types.h>
# include <carl/ptmvar.h>

static struct ptmvars 	pv;

run_ptm(unit, count, fp, arg)
	int		unit;
	unsigned long	count;
	int		(*fp)();
	char		*arg;
{
	/* init and start clock */
	pv.pv_tics	= count;
	pv.pv_fp	= fp;
	pv.pv_arg	= arg;
	ptm_start(unit, &pv);
}

stop_ptm()
{
	/* stop clock */
	pv.pv_fp	= 0;
	ptm_start(1, &pv);
}
