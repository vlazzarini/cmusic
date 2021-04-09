# define P_INTEGER
# include <carl/Player.h>


















struct mpu_resid {
	int	resid_iq_cc;		
	int	resid_tq_cc[10		];};















typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	unsigned short	ushort;


typedef	struct	_physadr { short r[1]; }
 *physadr;
typedef	struct	label_t	{
	int	val[13]; }
 label_t;

typedef	struct	_quad { long val[2]; }
 quad;
typedef	long	daddr_t;
typedef	char *	caddr_t;
typedef	u_long	ino_t;
typedef	long	swblk_t;
typedef	int	size_t;
typedef	int	time_t;
typedef	short	dev_t;
typedef	int	off_t;

typedef	struct	fd_set { int fds_bits[1]; }
 fd_set;




































extern int 	midi_out();
extern u_char   midi_in();
extern u_char  *midi_cmd_in();
extern char    *midi_pk();



struct	midi_par {
	u_char	par_index;	
	u_char	par_val;	
	u_char	par_ub;		
	char *	par_name;};


struct midi_spar {
	u_short	par_index;	
	u_short	par_val;	
	u_short	par_ub;		
	char *	par_name;};










































extern struct	midi_par dx7_ini_vox[];
extern u_short	dx7_ini_vox_len;
extern struct	midi_par dx7_ini_fun[];
extern u_short	dx7_ini_fun_len;
extern struct 	midi_par dx7_ch_info[];
extern u_short	dx7_ch_info_len;
extern struct	midi_par	dx7_xmit_ctl[];
extern u_short	dx7_xmit_ctl_len;
extern struct	midi_par	dx7_rcv_ctl[];
extern u_short	dx7_rcv_ctl_len;
extern struct	midi_par dx7_32v_vox[];
extern u_short	dx7_32v_vox_len;


struct dx7 {
	u_char	dx7_vox_val[		(32)	][	(155)	];	
	u_char dx7_fun_val[	(14)	];			
	u_char dx7_kbd_val[	(127)	];			
	u_char dx7_ctl_val[	(127)	];			
	u_char	dx7_nsv;				
	u_char	dx7_prg_val;				
	u_short	dx7_pbd_val;};


struct mpu_trk {
	unsigned char	*trk_buf;
	int		trk_len;
	int		trk_cnt;};

struct mpu_trkp {
	struct mpu_trk	*trkp_trk;
	int		trkp_pos;
	int		trkp_inc;};

typedef Player * Pptr;

Pptr 	P[8];
int	busy[8];
int	key;
int	keypressed;
struct mpu_trk mpu_trk[8];
int	cur_trk;
int	printit;
int	porta;

M_start() {
	extern Player *Instances();
	register int	i;
	Pptr	p;
	extern Player track();

	porta = 1;
	keypressed = 0;
	if (mpu_open(0, (0x04)		 | (0x02)		) != 0)
		Pexit(1);
	mpu_reset(0);
	
	if (mpu_set_track(0, (8		+0)	) != 0)
		exit(1);
	for (i = 0; i < 8; i++)
		setup_trk(&mpu_trk[i], (1024) );
	for (p = Instances(track), i = 0; p != NULL; p = Next_instance(p), i++) {
		P[i] = p;
		P[i]->P_pn[0] = i;
		busy[i] = 0;
	}

	cur_trk = 0;
	printit = 0;
	Pclock = 5000;

}


M_ter()
{
	stop_ptm();
	if (mpu_close(0))
		Pexit(5);
	exit(0);}

