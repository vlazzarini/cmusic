/* %M%	%I%	(CARL)	%G%	%U% */

# include <carl/carl.h>

/*
 **************************** MACROS **************************
 */
# ifdef P_FLOAT
# 	include <stdio.h>
# 	include <math.h>
# 	include <carl/libran.h>
# 	define P_DEBUG
# 	define OUTPROC cmusout
# 	define Forever -1.0
	/* macros for scheduler */
# 	define QLEN 100
# 	define TIME_QLEN 10.0 
# 	define PITCHES 12
# 	define Function FUNCTION
# 	define STOPSTR "Stop"
# 	define RESTR "r"
# 	define KEYSIG 'K'
# 	define TIMESIG 'T'
# 	define HOLDSTR "Hold"
# 	define KILLSTR "Kill"
#	define P_TYPE double
#	define ZERO 0.0
# else P_FLOAT
# 	define NULL 0
# 	define Forever -1
#	define P_TYPE long
#	define ZERO 0
# endif P_FLOAT
# define Return return(*Self)
/* macros for the running status of a Player */
# define P_NOSTAT 	0	/* illegal */
# define P_READY 	1	/* ready to run */
# define P_RUNNING 	2	/* the currently running process */
# define P_STOPPED 	3	/* can't run until made ready again */
# define P_KILLED 	4	/* Kill()'ed already */
# define P_STUNNED	5	/* on the way to being killed */
# define P_KILL		6	/* on its way to Kill() */
/* macros for the state of a test procedure via Wait_for() */
# define ONCE 		0
# define CONTINUOUS 	1
# define ALL_CHILDREN 	1
# define EACH_CHILD 	2
# define TRUE 1
# define True TRUE
# define FALSE 0
# define False FALSE
# define P_GT		0
# define P_GE		1
# define P_WF		1
# define P_WU		0
# define bit(x)		(1 << x)
# define P_STOP_SET	bit(0)
# define P_REST_SET	bit(1)
# define P_HOLD_SET	bit(2)
# define P_EXPR_ERR	bit(3)
# define P_NULL_PTR	bit(4)
# define P_NO_PARAM	bit(5)
# define P_WRAP		bit(6)
# define P_MEAS		bit(7)
# define P_TIMESIG	bit(8)
# define P_KEYSIG	bit(9)
# define P_METRONOME	bit(10)
# define P_CHORD	bit(11)
# define P_KILL_SET	bit(12)
# define P_WAIT_FOR	bit(13)
# define P_WAIT_UNTIL	bit(14)
# define P_TER_SET	bit(15)

/*
 **************************** Misc. Macros *************************
 */ 
# define NEW_PLAYER(name)New_player(name, Abs_time(), Forever, P_READY)

/* borrowed from <carl/defaults.h> */
/* MONO_IN_X, NCHANS should == 1, indicates function data monotonic in x */
# define H_MONO_IN_X	"MONO_IN_X"
/* XY_PAIRS, NCHANS should == 2, indicates function data in [x,y] pair format */
# define H_XY_PAIRS	"XY_PAIRS"

/*
 **************************** STRUCTURES **************************
 */
struct Player_P
{
	struct Player_P (*P_cproc)();/* pointer to player function */
	int     (*P_iproc)();	/* pointer to init function */
	char   *P_name;		/* its name */
	struct Player_P  *P_next;
	struct Player_P  *P_last;
	struct Player_P  *P_caller;	/* address of parent's instance data */
	char   *P_nstnc;	/* local instance structure tacked on here */
	short	P_children;	/* number of childern of this proc. */
	char  **P_ps;		/* string parameters */
	char  **P_pp;		/* postoperator string parameter */
	char  **P_pi;		/* string init parameters */
	P_TYPE *P_pn;		/* number parameters */
	P_TYPE  P_beg,		/* time player is slated to start */
	        P_dur,		/* time player is slated to end */
	        P_time;		/* current absolute time of the player */
	short   P_plen,		/* highest parameter */
	        P_hipn;		/* highest parameter to print */
	unsigned
		P_et:1,		/* select end test criterion */
                P_silent:1, 	/* if true, suppress default output */
                P_icode:1,	/* flag for once-only code */
                P_runstat:3,	/* status of player */
                P_reststat:1, 	/* suppress output temporarily */
		P_child_wait:2;	/* indicates waiting for children to exit */
};

typedef struct Player_P   Player;

struct wait_f 
{
	int     (*f_proc) ();	/* pointer to proc. that determines wait */
	Player * f_pp;		/* pointer to player owning this wait */
	unsigned        f_flag:1;
	struct wait_f
	               *f_next,
	               *f_last;
};

/*
 *********************ONLY IN P_FLOAT VERSIONS ************************
 */
# ifdef P_FLOAT
    struct n_list 
    {
	    struct headcl_hcl      *n_head;
	    struct headcl_hcl      *n_tail;
    };
    struct wait_u 
    {
	    int (*u_proc) ();	/* pointer to proc. that determines wait */
	    double   u_time;	/* or time when wait is to mature */
	    Player * u_pp;	/* pointer to player owning this wait */
	    unsigned 	u_divcnt;
	    unsigned        u_flag:1;
	    unsigned        u_void:1;
	    unsigned        u_wf:1;
	    struct wait_u  *u_next;
    };
    struct clist_cl 
    {
	    struct clist_cl
		   *cl_next,
		   *cl_last,
		   *cl_fbranch,
		   *cl_bbranch;
	    int     cl_fbcnt,	/* controlled variable foreward loop count */
		    cl_bbcnt,	/* controlled variable backward loop count */
		    cl_ftimes,	/* limit variable of foreward loop count */
		    cl_btimes;	/* limit variable of backward loop count */
	    char   *cl_datum,
		   *cl_meas_id;
	    int	cl_type;
	    unsigned	cl_chord:1;
	    unsigned	cl_stop:1;
	    unsigned	cl_hold:1;
	    unsigned	cl_kill:1;
    };


    struct sig_key {
	    char *key_sig;		/* string key signature */
	    char key_pc[PITCHES];	/* array of key signature accidentals */
	    char key_acc[PITCHES];	/* temporary accidentals */
    };

    struct sig_time {
	    char   *time_sig;	/* string time signature */
	    int	time_num;		/* numerator of signature */
	    int	time_den;		/* denominator of signature */
    };


    struct headcl_hcl 
    {
	    char   *hcl_nid;	/* addr. of char * for this cyclic list */
	    char   *hcl_raw;	/* undigested string */
	    char   *hcl_nn;	/* label of this cyclic list */
	    int     hcl_type;
	    int     hcl_len;
	    int	    hcl_incr;
	    int	meas_flag;		/* set when measure is parsed */
	    struct clist_cl
		   *hcl_nh,			/* head of parsed list */
		   *hcl_np,			/* pointer to next clist_cl */
		   *hcl_nt;			/* tail of parsed list */
	    char   *hcl_lastval;		/* for Hold instruction */
	    float   hcl_oct;			/* conservation of octave */
	    struct sig_key   hcl_aclist;	/* conservation of accidentals */
	    struct sig_time  *hcl_timesig;	/* time signature */
	    double		hcl_metro;	/* metronome coeficient */
    };
# else P_FLOAT
/*
 ********************ONLY IN P_INTEGER**************************
 */
    typedef struct prq_node {	/* the selected object or structure */
	    int prq_rank;
	    struct prq_node *prq_left, *prq_right;
	    int prq_dist;
	    Player *prq_po;
	    int prq_void;
    } **prq;			/*   prq is a ptr to a ptr to a node  	*/
# endif P_FLOAT

/*
 **************************** EXTERNS **************************
 */

extern long	Field_width, Precision;
extern double Scale[];
extern char    *malloc ();
extern char    *calloc ();
extern char    *realloc ();
extern long     _Pdebug;
extern int	Realtime;	/* if TRUE, run_arbiter() will sleep */
/* set by Rhythms, Pitches, Numbers, Strings and friends */
extern long Player_stat;
extern struct wait_f *wlist;	/* polled event wait list */
extern  Player * get_wait ();
extern  Player * mk_Player ();
extern  Player (*Get_paddr())();
extern  char   *Get_pname();
extern Wait_until();
extern void Wait_for();
extern char *CoerceToPname;

# ifdef P_FLOAT
extern struct wait_u tlist[QLEN];/* time wait list */
extern int      Argc;
extern char   **Argv;
extern  Function * Read_func ();
extern  Function * Gen ();
extern double   Func ();
extern double   Rel_func ();
extern double   add_player ();
extern struct	headcl_hcl	*getid();
extern  Player	read_stdin ();
extern  Player	poll_waits ();
extern struct	n_list    nlist;	/* cyclic lists */
extern double   Pitch ();
extern double   Octave ();
extern double   Pitches ();
extern double   Key ();
extern double   Keys ();
extern double   Rhythm ();
extern double   Rhythms ();
extern double   Expr ();
extern double   Exprs ();
extern double	Oct_pt ();
extern double	Oct_pts ();
extern double   Abs_time ();
extern double   Rel_time ();
extern double   Beg_time ();
extern double   Dur_time ();
extern double   End_time ();
extern double   Scale[];
extern double   Tempo ();
extern double   P ();
extern char    *Ps ();
extern char    *Pp ();
extern int      trim_zeros;
extern char    *Strings ();
extern char    *Peekcl ();
extern int      Motive ();
extern char    *Index ();
extern char    *Get_list ();
extern char    *Getcladdr ();
extern int      Rd_motive ();
extern char     notend;		/* set by notes() and rhythms() */
extern char     noterest;	/* set by notes() */
extern int      cmusout ();
extern struct	sig_time *Get_timesig();
extern struct	sig_key *Get_sig_key();
extern double	Get_metronome();
extern double   Now;
extern double	Pclock;
extern int      (*Print_notes) ();
# else P_FLOAT
extern long	Now;
extern long	Pclock;
# endif P_FLOAT

extern  Player * New_player ();
extern  Player * Caller ();
extern  Player * Self;
extern  Player * Instances ();
#define Next_instance(x) x->P_next

/*
 **************************** SYMBOL TABLE **************************
 */
struct symtab {
	int 		ss_type;
	char *		ss_name;
	struct hlist **	ss_hlist;
	unsigned	ss_collisions;
	unsigned	ss_installs;
};

extern struct symtab symtab[];	/* from symtab.c */

/* symbol tables */
/* these are hashed by their memory address */
# define ST_CLASS	0	/* Player class template data */
# define ST_CYCLIST	1	/* cyclic lists */
/* these are hashed by the value of their string name */
# define ST_CYCLABL	2	/* cyclic list label */
# define ST_STR_CLASS	3	/* Player class template data */

# define AT_CLASS	&symtab[ST_CLASS]
# define AT_CYCLIST	&symtab[ST_CYCLIST]
# define AT_CYCLABL	&symtab[ST_CYCLABL]
# define AT_STR_CLASS	&symtab[ST_STR_CLASS]

/* prime number for symbol table length helps the hashing function */
# define HSIZE		61

/*
 * types of things that are managed
 * by a symbol table. each has its
 * own symbol table.
 */
union vals {
	Player * st_class;
	struct wait_f  *st_waitf;
	struct wait_u  *st_waitu;
	struct headcl_hcl      *st_cycl;
};

/*
 * structure of a symbol table
 * entry.
 */
struct hlist {
	char   *hl_sym;		/* symbolic name */
	struct hlist   *hl_next;/* next hash entry */
	union vals hl_val;	/* value to store */
};

extern struct hlist    *lookup ();
extern struct hlist    *install ();
