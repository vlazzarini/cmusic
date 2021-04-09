/* %M%	%I%	(CARL)	%G%	%U% */

# include <carl/sa_mpuvar.h>
# include <sys/types.h>
# include <carl/midi.h>
# include <carl/dx7.h>

mpu_record(mpu_unit, rbuf, rlen)
	int	mpu_unit;
	char	*rbuf;
	int	rlen;
{
	extern int	porta;
	extern int	printit;
	extern int	keypressed;
	char		ibuf[32];
	register int	i, n, len = 0, record = 0;

	mpu_set_record_mode(mpu_unit);
	if (!porta) {
		record = 1;
		keypressed = 0;
	}

	while ((n = mpu_readmsg(mpu_unit, ibuf, sizeof(ibuf))) > 0) {
		int ch;
		if (porta) {
			    /* skip until portamento foot switch is depressed */
			if (n==4 && (ibuf[1] & 0xf0) == CH_CTL 
			    && ibuf[2] == CTL_DX7_PORTA_FSW) {
				/* portamento footswitch has changed */
				if (ibuf[3] == 0x7f) {	/* switch down */
					printf("<\t");
					record = 1;
					continue;	/* skip this command */
				} else {
					printf(">%d\n", len);
					break;		/* and this one */
				}
			}
		} else {
			if (printit == 4)
				printf("(%d)", keypressed);
			if (keypressed != 0) {
				printf("%c}%d\n", ch, len);
				break;
			}
		}
		if (record == 1) {
			for (i = 0; ((i < n) && (len < rlen)); i++) {
				if (printit == 4)
					fprintf(0, "%x ", ibuf[i] & 0xff);
				rbuf[len++] = ibuf[i];
			}
			if (printit == 4)
				fprintf(0, "\n");
		}
	}
	mpu_reset(mpu_unit);
	return(len);
}

static unsigned char recmode[] = { 
		0xff,	/* reset */
		0x33,	/* thru off */
		0x88,	/* midi thru off */
		0x87,	/* pitch bender on */
		0x26	/* start recording, stop playing */
};


mpu_set_record_mode(mpu_unit)
{
	/* set command track to turn on MPU in record mode */
	if (mpu_set_track(mpu_unit, MPU_TR_COM) != 0)
		exit(3);
	/* reset mpu, and fire it up in record mode */
	if (mpu_write(mpu_unit, recmode, sizeof(recmode)) != sizeof(recmode))
		exit(2);
}
