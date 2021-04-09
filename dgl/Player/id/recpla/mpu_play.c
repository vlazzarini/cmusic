/* %M%	%I%	(CARL)	%G%	%U% */

# include <carl/sa_mpuvar.h>
# include <sys/types.h>
# include <carl/midi.h>
# include <carl/dx7.h>

# define min(a,b) (((a)<(b))?(a):(b))


mpu_play(mpu_unit, track, pla_buf, pla_cnt) 
	int 	mpu_unit,
		track;
	char 	*pla_buf;
	int	pla_cnt;
{
	int	n, cnt, pos, first = 0;
	char	ibuf[32];

	/* select playback track */
	if (mpu_set_track(mpu_unit, track) != 0)
		exit(1);
	/* send data to mpu */
	for (cnt = min(128, pla_cnt), pos = 0;
	     pos < pla_cnt;
	     pos += n, cnt = min(128, pla_cnt - pos)) {
		if ((n = mpu_write(mpu_unit, &pla_buf[pos], cnt)) < 0)
			exit(2);
		if (first == 0) {
			mpu_start_play_mode(mpu_unit, track);
			first = 1;
		}
	}

	/* wait for fc from mpu */
	while ((n = mpu_readmsg(mpu_unit, ibuf, sizeof(ibuf))) > 0) {
		if (ibuf[0] == 0xfc)
			break;
	}
}

static unsigned char playmode[] = { 
		0xff,	/* reset */
		0xec,	/* set track */
		0x01,	/* track == 0 , overwritten by current track # */
		0xb8,	/* clear play counters */
		0x87,	/* bender on */
		0x0a	/* start play */
};

mpu_start_play_mode(mpu_unit, track)
{
	/* set command track to turn on MPU in playback mode */
	if (mpu_set_track(mpu_unit, MPU_TR_COM) != 0)
		exit(3);
	/* start playback */
	playmode[2] = 1 << track;
	if (mpu_write(mpu_unit, playmode, sizeof(playmode))!=sizeof(playmode))
		exit(4);
	/* select playback track */
	if (mpu_set_track(mpu_unit, track) != 0)
		exit(1);
}
