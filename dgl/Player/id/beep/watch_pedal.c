/* watch_pedal.c	1.1	(CARL)	8/9/85	14:49:08 */

/*
* very smart watch pedal routine called by mpu_cint_dgl()	
* on MPU interrupt
* it needs mpuio.o
*/

#include "xc_mpu.h"

extern  which_mpu;		/* nb of the MPU which interrupted	 
				*/

u_char status;			/* current runing status */

u_char rec_buf[4];

int     sustain_pedal = P_REPOS ;
int     portamento_pedal = P_REPOS ;


watch_pedal (data)
register    u_char data;
{

    rec_buf[0] = data;
    if (data == 0xf8) {		/* time overflow	 */
/*	printf("ovfl\n");		*/
	return;
    }

    else {
    /* it was a time tag		 */
    /* let's read the next byte	 */
	rec_buf[1] = data = mpuread (which_mpu);
    /* either key or control */

	if (!(data & 0x80)) {	/* the status byte is missing */
	    if ((status >= 0xc0) && (status <= 0xdf)) {/* pr chg */
		return;
	    }

	    else {		/* 3 bytes		 */
		rec_buf[2] = mpuread (which_mpu);/* velocity	 */
		if ((status & 0xf0) == 0xb0) {/* control */
		    switch ( data ) {
			case 0x40: /* sustain pedal	 */
				sustain_pedal = rec_buf[2] ;
			    break;
			case 0x41: /* portamento	 */
			    portamento_pedal = rec_buf[2];
			    break;
			default: 
			    break;
		    }
		    return;
		}
	    }
	}
	else {			/* status byte is there		 */
	    status = data;
	    if (data < 0xf0) {	/* status byte	 */
		    rec_buf[2] = mpuread (which_mpu);
		if ((data >= 0xc0) && (data <= 0xdf)) {
		/* 2 bytes cmd */
		    return;
		}
		else {	/* three bytes command		*/
		    rec_buf[3] = mpuread (which_mpu);
		    if ((data & 0xf0) == 0xb0) {/* control */
/*			printf("control %x\n",rec_buf[2]) ;	*/
			switch (rec_buf[2]) {
			    case 0x40: /* sustain pedal	 */
				sustain_pedal = rec_buf[3] ;
				break;
			    case 0x41: /* portamento	 */
				portamento_pedal = rec_buf[3];
				break;
			    default: 
				break;
			}
			return;
		    }

		}
	    }

	    else {		/* it is an mpu mark	 */
		return;

	    }
	}
    }
}
