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

extern Pptr 	P[8];
extern int	busy[8];
extern int	key;
extern struct mpu_trk mpu_trk[8];
extern struct mpu_trkp mpu_trkp[8];
extern int	cur_trk;
extern int	printit;

typedef struct mpu_trkp trkpstruct;struct track {
 int	pos, cnt, len, sequence; u_char	*pla_buf; int	ct; trkpstruct tp;
};
	

Player
track(_x)
	Player *_x;
{
	unsigned char	cmd_buf[4]; 
	int		i, n; if(_x->P_icode==0){
		((struct track *)_x->P_nstnc)->sequence = -1;

}


start:	if (((struct track *)_x->P_nstnc)->sequence < 0) {
		((struct track *)_x->P_nstnc)->ct = _x->P_pn[0];			
		fprintf(0, "(%d", _x->P_pn[2]);
		if (mpu_trk[((struct track *)_x->P_nstnc)->ct].trk_cnt == 0) {
			printf("empty track\n");
			Stop(Self);
			Return;
		}

		((struct track *)_x->P_nstnc)->pla_buf = mpu_trk[((struct track *)_x->P_nstnc)->ct].trk_buf;
		((struct track *)_x->P_nstnc)->cnt	= mpu_trk[((struct track *)_x->P_nstnc)->ct].trk_cnt;
		init_trkp(&((struct track *)_x->P_nstnc)->tp, &mpu_trk[((struct track *)_x->P_nstnc)->ct]);
		((struct track *)_x->P_nstnc)->pos	= 0;
		((struct track *)_x->P_nstnc)->sequence = 0;
		busy[_x->P_pn[2]] = 1;

}


again:	if (((struct track *)_x->P_nstnc)->sequence == 0) { 
		register int	next = 0, p;
		while ((((struct track *)_x->P_nstnc)->len = next_cmd(&((struct track *)_x->P_nstnc)->tp)) > 0) {
			((struct track *)_x->P_nstnc)->pos = p = ((struct track *)_x->P_nstnc)->tp.trkp_pos;
			if (((struct track *)_x->P_nstnc)->pla_buf[p] == 0xf8)
				next += 240;
			else if (((struct track *)_x->P_nstnc)->pla_buf[p+1] == 0xf9)
				
				next += ((struct track *)_x->P_nstnc)->pla_buf[p];
			else {
				next += ((struct track *)_x->P_nstnc)->pla_buf[p];
				break;
		}

		}

		if (((struct track *)_x->P_nstnc)->len < 0) {
			fprintf(0, "track: error\n");
			((struct track *)_x->P_nstnc)->len = 0;
		}

		if (((struct track *)_x->P_nstnc)->len == 0) {
			((struct track *)_x->P_nstnc)->sequence = -1;
			if (_x->P_pn[1] != 0)
				goto start;
			Stop(Self);
			fprintf(0, "%d)", _x->P_pn[2]);
			busy[_x->P_pn[2]] = 0;
			Return;
		}

		if (printit == 1)
			fprintf(0, "%d next=%d\n", ((struct track *)_x->P_nstnc)->ct, next);
		Wait_until(Now + next);
		((struct track *)_x->P_nstnc)->sequence = 1; }
 else {
		
		cmd_buf[0] = 0xd0;
		if (printit == 1)
			fprintf(0, "%d\t", ((struct track *)_x->P_nstnc)->ct);
		cmd_buf[1] = (((struct track *)_x->P_nstnc)->pla_buf[((struct track *)_x->P_nstnc)->pos+1] & 0xf0) | ((struct track *)_x->P_nstnc)->ct;
		for (i = 2; i < ((struct track *)_x->P_nstnc)->len; i++) {
			cmd_buf[i] = ((struct track *)_x->P_nstnc)->pla_buf[((struct track *)_x->P_nstnc)->pos+i];	
			if (printit == 1) 
				fprintf(0, "%x\t", ((struct track *)_x->P_nstnc)->pla_buf[((struct track *)_x->P_nstnc)->pos+i] & 0xff);
		}

		if (printit == 1)
			fprintf(0, "\n");
		if (printit == 0) {
			if ((n = mpu_write(0, cmd_buf, ((struct track *)_x->P_nstnc)->len)) < 0) {
				fprintf(0, "mpu_write: %d\n", n);
				Pexit(1);
		}

		}

		((struct track *)_x->P_nstnc)->sequence = 0;
		goto again;
}
	return(*_x);
}
/* initialize Player instance */
PI_track(i, c)
	Player *i, *c;
{
	i->P_cproc = c->P_cproc;
	i->P_iproc = c->P_iproc;
	i->P_name = c->P_name;
	i->P_hipn = c->P_hipn;
	i->P_plen = c->P_plen;
	i->P_pn=(long *) calloc(3, (unsigned) sizeof(long));
	i->P_ps=(char **)calloc(3, (unsigned) sizeof(char *));
	i->P_pp=(char **)calloc(3, (unsigned) sizeof(char *));
	i->P_nstnc=(char *)calloc(1, (unsigned) sizeof(struct track));
}
/* make Player class template */
PC_track(c)
	Player *c;
{
	c->P_cproc = track;
	c->P_iproc = PI_track;
	c->P_name = "track";
	c->P_hipn = c->P_plen = 2;
	c->P_children =  8;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* preset Player instances */
PP_track(c)
	Player	*c;
{
	Player	*i = mk_Player(c);
	PI_track(i, c);
	i->P_beg	= (long) 0;
	i->P_time	= (long) 0;
	i->P_dur	= (long)  Forever;
	i->P_runstat	=  P_STOPPED;
	set_wait(i, i->P_time);
}
