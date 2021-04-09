/* %M%	%I%	(CARL)	%G%	%U% */

#define MIDI_MAX_CHANS	(16)		/* maximum midi channels */

/* masks */
#define M_CMD		(0x80)		/* midi command bit mask */
#define M_CMD_MASK	(0xF0)		/* midi command mask */
#define M_CHAN_MASK	(0x0F)		/* midi channel mask */
#define M_VAL_MASK	(0x7F)		/* midi value mask */

/* command ids */
/* CH_... == channel commands */
#define CH_KEY_OFF	(0x80)		/* channel key off */
#define CH_KEY_ON	(0x90)		/* channel key on */
#define CH_POLY_KPRS	(0xA0)		/* polyphonic key pressure */
#define CH_CTL		(0xB0)		/* channel control */
#define CH_PRG		(0xC0)		/* channel program change */
#define CH_PRESSURE	(0xD0)		/* channel pressure */
#define CH_P_BEND	(0xE0)		/* channel pitch bend */

/* SX_... == system exclusive commands */
#define SX_CMD		(0xF0)		/* system exclusive command */
#define SX_EOB		(0xF7)		/* system exclusive end of block */

/* SC_... == system common commands */
#define SC_MSEL		(0xF2)		/* system common measure select */
#define SC_SSEL		(0xF3)		/* system common song select */
#define SC_TSEL		(0xF6)		/* system common tune request */

/* RT_... == realtime commands */
#define RT_TCIP		(0xF8)		/* rt timing clock in play */
#define RT_TCWME	(0xF9)		/* rt timing clock w/ meas. end */
#define RT_SA1M		(0xFA)		/* rt start at first measure */
#define RT_CONT		(0xFB)		/* rt continue start */
#define RT_TCIS		(0xFC)		/* rt timing clock in stop */
#define RT_RESET	(0xFF)		/* rt system reset */

/* default for reading/writing under regular UNIX */
#define MIDI_DEV     "/dev/ttya"
extern int 	midi_out();
extern int	midi_in();
extern u_char  *midi_cmd_in();
extern char    *midi_pk();

/* template for all midi parametric data */
/* note: watch out for synth. parameters that exceed u_char range! */
typedef struct	midi_par {
	u_char	par_index;	/* parameter index in hardware table */
	u_char	par_val;	/* parameter current value */
	u_char	par_ub;		/* parameter upper boundary */
	char *	par_name;	/* parameter name */
} MidiPar;

/* same as above, but for parameters requiring u_short storage */
typedef struct midi_spar {
	u_short	par_index;	/* parameter index in hardware table */
	u_short	par_val;	/* parameter current value */
	u_short	par_ub;		/* parameter upper boundary */
	char *	par_name;	/* parameter name */
} MidiSpar;

# define TRW_SEL_TTY	1
# define TRW_SEL_RFD	2
# define TRW_SEL_WFD	3
