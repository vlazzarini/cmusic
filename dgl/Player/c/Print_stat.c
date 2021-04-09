# include <carl/Player.h>

Print_stat(fd, msg)
# ifdef P_FLOAT
	FILE	*fd;	/* file descriptor for output */
# else P_FLOAT
	int	fd;
# endif P_FLOAT
	char	*msg;	/* string message to prepend to diagnostic output */
/*
 * Pretty-print bit-fields of \fBPlayer_stat\fP.
 */
{
	register int	i, first = 1;
	register char	*c;

	fprintf(
# ifdef P_FLOAT
		fd,
# else P_FLOAT
		1,
# endif P_FLOAT
		"%s:\tPlayer_stat=%d<",
		msg, Player_stat);


	for (i = 0; i < 32; i++) {
		switch (Player_stat & 1 << i) {
			case 0:		 continue;
			case P_STOP_SET: c = "P_STOP_SET"; break;
			case P_REST_SET: c = "P_REST_SET"; break;
			case P_HOLD_SET: c = "P_HOLD_SET"; break;
			case P_EXPR_ERR: c = "P_EXPR_ERR"; break;
			case P_NULL_PTR: c = "P_NULL_PTR"; break;
			case P_NO_PARAM: c = "P_NO_PARAM"; break;
			case P_WRAP:	 c = "P_WRAP"; break;
			case P_MEAS:	 c = "P_MEAS"; break;
			case P_TIMESIG:	 c = "P_TIMESIG"; break;
			case P_KEYSIG:	 c = "P_KEYSIG"; break;
			case P_METRONOME:c = "P_METRONOME"; break;
			case P_CHORD:	 c = "P_CHORD"; break;
		}
		fprintf(
# ifdef P_FLOAT
			fd,
# else P_FLOAT
			1,
# endif P_FLOAT
			first?"%s":"|%s", c);
		first = 0;
	}
	fprintf(
# ifdef P_FLOAT
		fd,
# else P_FLOAT
		1,
# endif P_FLOAT
		">\n");
}
