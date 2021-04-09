# include <carl/Player.h>

Send(inst, time)
	register Player *inst;
	register P_TYPE time;
{
	Player_stat &= ~(P_NULL_PTR | P_NO_PARAM);
	if (vp == NULL) {
		Player_stat = P_NULL_PTR;
		return(-1);
	}
		
	vp->P_pn = Self->P_pn;
	vp->P_pp = Self->P_pp;
	vp->P_pi = Self->P_pi;
	vp->P_ps = Self->P_ps;
	vp->P_plen = Self->P_plen;
	vp->P_hipn = Self->P_hipn;
	return(0);
}
