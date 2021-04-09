/* %M%	%I%	(CARL)	%G%	%U% */

#include <carl/Player.h>

/***********************PUBLISHED**********************/ 
P_TYPE	Now;			/* global current time */
P_TYPE	Pclock;			/* clock resolution in ticks/second */
int     (*Print_notes) ();	/* proc. for default Player output */
Player * Self;			/* current instance structure pointer */
long    Player_stat;
int     Realtime;		/* if TRUE, run_arbiter() will sleep */
# ifdef P_FLOAT
int     Argc; 			/* global pointers to arguments */
char  **Argv;
char   *CoerceToPname;		/* name to substitute in note-statement */
long Field_width = 6, Precision = 3;	/* control printout in cmusout */
double Scale[]
/*
 * Array of Hz values corresponding to the equal-tempered
 * semitone pitches of the middle octave of the piano.
 */
	=
{
	261.625565,	/* c4 */
	277.182631,	/* cs4 */
	293.664768,	/* d4 */
	311.126984,	/* ds4 */
	329.627557,	/* e4 */
	349.228231,	/* f4 */
	369.994423,	/* fs4 */
	391.995436,	/* g4 */
	415.304698,	/* gs4 */
	440.000000,	/* a4 */
	466.163762,	/* as4 */
	493.883301	/* b4 */
};
# endif P_FLOAT

/***********************UNPUBLISHED**********************/ 
struct wait_f  *wlist;		/* polled event wait list */
int	return_on_meas_id;

# ifdef P_FLOAT
struct n_list   nlist;		/* cyclic lists */
struct wait_u   tlist[QLEN];	/* time wait list */
# endif P_FLOAT

# ifdef P_DEBUG
long    _Pdebug;		/* internal debugging */
# endif P_DEBUG

init_globs () {
	Now = ZERO;
	Self = NULL;
# ifdef P_FLOAT
	Print_notes = OUTPROC;
	nlist.n_head = NULL;
	nlist.n_tail = NULL;
	Pclock = .1;		/* set polling clock interval */
# else P_FLOAT
	Print_notes = NULL;
	Player_stat = 0;
	Realtime = 1;
# endif P_FLOAT
# ifdef P_DEBUG
	_Pdebug = 0;
# endif P_DEBUG
	return_on_meas_id = 0;
}
