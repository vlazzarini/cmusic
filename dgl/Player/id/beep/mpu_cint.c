/* mpu_cint_dgl.c	1.1	(CARL)	8/9/85	14:47:44 */

/* dgl version of C mpu interrupt routine		*/
/* minimalist phylosophy				*/

# include "xc_mpu.h"

/*
* called by mpu_asint upon interrupt, try to do the less possible
* and go back to watch TV
*/

mpu_cint_dgl(data)
unsigned register char   data;
{	

    int     track;
	extern which_mpu ;
	extern watch_pedal() ;

    if (data < 0xf0) {		/* record data	 */
		watch_pedal( data) ;
		return ;
    }

    else {
	if (data <= 0xf7) {	/* track play request	 */
		return ;/* should send an 0x0,0xfc to stop play*/
	}

	else {
	    switch (data) {
		case 0xf8: 	/* timing overflow	 */
			watch_pedal( data) ;/* useless but to be coherent*/
		    break;
		case 0xf9: 	/* conduct request	 */
		    break;
		case 0xfd: 
		    break;
		case 0xfc: 	/* data end		 */
		    break;
		case 0xff: 	/* system exclusif		 */
		    break;

	    }
	}
 }
}

