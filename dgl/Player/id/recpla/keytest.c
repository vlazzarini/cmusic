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
extern int	keypressed;
extern struct mpu_trk mpu_trk[8];
extern int	cur_trk;
extern int	printit;

keydown()
{
	int	ch;

	if (nb_acia_read(0, &ch) != 0) {
		acia_write(0, ch);
		key = ch;
		return(keypressed = TRUE); }
 else
		return(FALSE);

}
struct keytest {
 int tmpvar;
};


Player
keytest(_x)
	Player *_x;
{
	extern int	porta; if(_x->P_icode==0){
		Wait_for(keydown, CONTINUOUS);
		Return;

	}


	switch (key) {
		register int	i, j;
		case '0':	
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			((struct keytest *)_x->P_nstnc)->tmpvar = key - '0';
			break;
		case 'P':	
			printit = ((struct keytest *)_x->P_nstnc)->tmpvar;
			break;
		case 'R':	
			mpu_reset(0);
			
			if (mpu_set_track(0, (8		+0)	) != 0)
				exit(1);
		case 'S':	
			{
			register int	i;
			for (i = 0; i < 8; i++)
				Set_p(P[i], 1L, 0L);
			break;
		}

		case '.':	
			Pexit();
		case 'c':	
			{
			register int pq;
			if ((pq = getpfree()) == -1) {
				fprintf(0, "all busy\n");
				break;
			}

			cur_trk = ((struct keytest *)_x->P_nstnc)->tmpvar;
			
			Set_p(P[pq], 0L, cur_trk);
			
			Set_p(P[pq], 1L, 1);
			Start(P[pq]);
			printf("%d\n", pq);
			break;
		}

		case 'd':	
			cur_trk = ((struct keytest *)_x->P_nstnc)->tmpvar;
			dump_trk(&mpu_trk[cur_trk]);
			break;
		case 'h':
			help();
			break;
		case 'l':	
			cur_trk = ((struct keytest *)_x->P_nstnc)->tmpvar;
			list_trk(&mpu_trk[cur_trk]);
			break;
		case 'p':
			if (porta)
				porta = 0;
			else
				porta = 1;
			break;
		case 'r':	
			cur_trk = ((struct keytest *)_x->P_nstnc)->tmpvar;
			mpu_trk[cur_trk].trk_cnt =
			    mpu_record(0,
				mpu_trk[cur_trk].trk_buf,
				mpu_trk[cur_trk].trk_len);
			break;
		case 's':	
			{
			register int pq;
			if ((pq = getpfree()) == -1) {
				fprintf(0, "all busy\n");
				break;
			}

			cur_trk = ((struct keytest *)_x->P_nstnc)->tmpvar;
			
			Set_p(P[pq], 0L, cur_trk);
			
			Set_p(P[pq], 1L, 0);
			
			Set_p(P[pq], 2L, pq);
			Start(P[pq]);
			printf("%d\n", pq);
			break;
		}

		case 'x':	
			cur_trk = ((struct keytest *)_x->P_nstnc)->tmpvar;
			Set_p(P[cur_trk], 1L, 0);
			break;
		default:
			acia_write(0, '?');
			break;
}


	return(*_x);


}

getpfree()
{
	register int	pfree;

	for (pfree = 0; pfree < 8; pfree++) {
		if (!busy[pfree])
			return(pfree);
	}

	return(-1);

}


help()
{
	printf(
	    "0	set buffer n",
	    "P	set/clear printit n",
	    "R	mpu reset n",
	    "S	stop all n",
	    ".	quit n",
	    "c	continuous loop n",
	    "d	dump track n",
	    "l	list track n",
	    "r	record n",
	    "s	start n",
	    "x	cancel track n"
	);}

/* initialize Player instance */
PI_keytest(i, c)
	Player *i, *c;
{
	i->P_cproc = c->P_cproc;
	i->P_iproc = c->P_iproc;
	i->P_name = c->P_name;
	i->P_hipn = c->P_hipn;
	i->P_plen = c->P_plen;
	i->P_pn=(long *) calloc(1, (unsigned) sizeof(long));
	i->P_ps=(char **)calloc(1, (unsigned) sizeof(char *));
	i->P_pp=(char **)calloc(1, (unsigned) sizeof(char *));
	i->P_nstnc=(char *)calloc(1, (unsigned) sizeof(struct keytest));
}
/* make Player class template */
PC_keytest(c)
	Player *c;
{
	c->P_cproc = keytest;
	c->P_iproc = PI_keytest;
	c->P_name = "keytest";
	c->P_hipn = c->P_plen = 0;
	c->P_children = 1;
	if (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)
		Pexit(101);
	if(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)
		Pexit(101);
}
/* preset Player instances */
PP_keytest(c)
	Player	*c;
{
	Player	*i = mk_Player(c);
	PI_keytest(i, c);
	i->P_beg	= (long) 0;
	i->P_time	= (long) 0;
	i->P_dur	= (long) -1L;
	i->P_runstat	= P_READY;
	set_wait(i, i->P_time);
}
