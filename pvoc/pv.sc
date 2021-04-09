 include <carl/cmusic.h>

/* This is an example of a cmusic score for resynthesizing phase
 * 	vocoder analysis data.  To use it as a template for your
 *	own resynthesis, simply copy this file, and make whatever
 *	changes you wish.  At the very least, you will have to
 *	change FILE to the filename of your analysis data, and
 *	PV_D to the decrement (D) value for your analysis.  You will
 *	also probably want to change the duration DUR and the
 *	number of note statements.
 */

/*
 * RATE is the cmusic output sampling rate.
 */

# define RATE	(16K)

/*
 * PV_D is given in the summary file as D.  This value is used
 * 	to determine the number of values which will be (linearly)
 * 	interpolated between successive analysis values.  Setting
 * 	PV_D to some value other than the D used in the analysis
 * 	will have the effect of performing time-scaling.  The fact
 *	that this interpolation is linear (as opposed to ideal) is
 *	the major source of error in cmusic resynthesis.
 */

# define PV_D	(40)

/*
 * DUR is the duration of the original sound which was analyzed
 * 	by the phase vocoder.  
 */

# define DUR	(6)

/*
 * TIMEVAR is the time change factor.  (Higher numbers expand time,
 * 	lower numbers contract time, and TIMEVAR=1 leaves it unaltered.)
 * 	To expand by a factor of two, set TIMEVAR=2.
 */

# define TIMEVAR	(1)

/*
 * PITCHVAR is the pitch change factor.  (Higher numbers increase pitch,
 * 	lower numbers decrease pitch, and PITCHVAR=1 leaves it unaltered.)
 * 	To raise pitch by one half step, set PITCHVAR=1.06.
 */

# define PITCHVAR	(1)

/*
 * FILE is the soundfile with the interleaved analysis data from the
 * 	phase vocoder.
 */

# define FILE	"/sndb/rusty/voice.i"

/*
 * ARATE is the analysis sampling rate.
 */

# define ARATE	((RATE/PV_D)/TIMEVAR)

# define INC	(ARATE/RATE)

var 0 s1 FILE ;

set rate = RATE ;

/*
 * The instrument pv resynthesizes a single channel of phase vocoder
 * 	analysis data.  The first sndfile reads the amplitude data, and
 * 	second sndfile reads the frequency data.  Hence, p5 will always
 * 	be an odd number and p6 will always be that number plus one.
 */

ins 0 pv ;
/*	out	amp	inc	file	chan	start	end	d	d ; */
sndfile	b1	1	INC	s1	p5	0	-1	d	d ;
sndfile	b2	1	INC	s1	p6	0	-1	d	d ;

/*	out	in	in */
mult	b3	b2	(PITCHVAR)Hz ;

/*	out	amp	inc	table	d ; */
osc	b4	b1	b3	f1	d ;

out	b4 ;
end ;

/*
 * f1 is a sine wave of amplitude .5; a sine wave of amplitude 1 may
 * cause samples out of range depending on the bandwidth of the analysis
 * filters - if the filters overlap significantly, they may appear to
 * contain more energy than the original signal.
 */

GEN5(f1) 1, 0.5, 0 ;

/* 	p3	p4		p5		p6		p7	*/
note 0	pv	DUR*TIMEVAR	1		2 ;
note 0	pv	DUR*TIMEVAR	3		4 ;
note 0	pv	DUR*TIMEVAR	5		6 ;
note 0	pv	DUR*TIMEVAR	7		8 ;
note 0	pv	DUR*TIMEVAR	9		10 ;
note 0	pv	DUR*TIMEVAR	11		12 ;
note 0	pv	DUR*TIMEVAR	13		14 ;
note 0	pv	DUR*TIMEVAR	15		16 ;
note 0	pv	DUR*TIMEVAR	17		18 ;
note 0	pv	DUR*TIMEVAR	19		20 ;
note 0	pv	DUR*TIMEVAR	21		22 ;
note 0	pv	DUR*TIMEVAR	23		24 ;
note 0	pv	DUR*TIMEVAR	25		26 ;
note 0	pv	DUR*TIMEVAR	27		28 ;
note 0	pv	DUR*TIMEVAR	29		30 ;
note 0	pv	DUR*TIMEVAR	31		32 ;
note 0	pv	DUR*TIMEVAR	33		34 ;
note 0	pv	DUR*TIMEVAR	35		36 ;
note 0	pv	DUR*TIMEVAR	37		38 ;
note 0	pv	DUR*TIMEVAR	39		40 ;
note 0	pv	DUR*TIMEVAR	41		42 ;

ter ;
