/* %M%	%I%	(CARL)	%G%	%U% */

/* %M%	%I%	(CARL)	%G%	%U% */

/*
 * next_cmd -	set up mpu_trkp structure to point to next mpu command
 * takes:
 * 	struct mpu_trkp	*trkp;	pointer to track structure
 * returns:
 * 	number of bytes in next mpu command (0 if done), or
 * 	-1 on illegal timetag ( > 0xef)
 * side effects:
 * 	sets track position and length fields on mpu_trkp structure.
 */

# include "medit.h"
# define u_char	unsigned char

next_cmd(trkp)
	struct mpu_trkp	*trkp;
{
	extern int	printit;
	register int 	cnt  = trkp->trkp_trk->trk_cnt, 
			i;
	register u_char	*buf = trkp->trkp_trk->trk_buf;
	int		rtn = 0;

	i = trkp->trkp_pos + trkp->trkp_inc; 
	if (i < cnt) {
		if (printit == 5)
			printf("buf[%d]=%x\n", i, buf[i] & 0xff);
		if (buf[i] == (u_char) 0xf8) {		/* mpu measure */
			trkp->trkp_pos = i;
			trkp->trkp_inc = rtn = 1;
		} else if (buf[i] > (u_char ) 0xef) {
			rtn = -1;			/* illegal timetag */
		} else if ((buf[i+1] & (u_char) 0xf0) == (u_char) 0xf0) {	
			trkp->trkp_pos = i;		/* mpu mark */
			trkp->trkp_inc = rtn = 2;
		} else if (buf[i+1] >= 0xc0 && buf[i+1] <= 0xdf) {
			trkp->trkp_pos = i;
			trkp->trkp_inc = rtn = 3;
		} else {
			trkp->trkp_pos = i;
			trkp->trkp_inc = rtn = 4;
		}
	}
	return(rtn);
}

/*
 * unsigned char x[] = {0xf8, 0xde, 0xf9, 0xf8, 0xf8, 0x0, 0xf9, 0xf8, 
 * 	0x78, 0x90, 0x3c, 0x3c};
 * 
 * struct mpu_trk	trk = {
 * 		x,
 * 		12,
 * 		12,
 * };
 * struct mpu_trkp trkp = {
 * 		&trk,
 * 		0,
 * 		0
 * };
 * 
 * int printit = 5;
 * 		
 * main()
 * {
 * 	int	len;
 * 
 * 	while ((len = next_cmd(&trkp)) > 0)
 * 		continue;
 * }
 */
