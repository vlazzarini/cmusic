/* %M%	%I%	(CARL)	%G%	%U% */

struct mpu_trk {
	unsigned char	*trk_buf;
	int		trk_len;
	int		trk_cnt;
};

struct mpu_trkp {
	struct mpu_trk	*trkp_trk;
	int		trkp_pos;
	int		trkp_inc;
};

# define MPU_TRK_LEN (1024) 
# define MAL_ARENA_SIZE (MPU_TR_MAX*MPU_TRK_LEN)
# define NARGS (3)
# define P_TRK	8
