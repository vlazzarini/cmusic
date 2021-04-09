/* lib_smart_dgl.c	1.1	(CARL)	8/9/85	14:47:28 */

/* library for functions		*/

#include "xc_mpu.h"

extern sustain_pedal ;/* in watch_pedal.c		*/
extern portamento_pedal ;

start_watch_pedal(mpu_nb) 
{
	sustain_pedal = portamento_pedal = P_REPOS ;
	m_clrrec(mpu_nb) ;
	m_startrec(mpu_nb) ;
	return(1) ;
}
	

stop_watch_pedal(mpu_nb)
{
	m_stoprec(mpu_nb) ;
	sustain_pedal = portamento_pedal = P_REPOS ;
	return(1) ;
}
