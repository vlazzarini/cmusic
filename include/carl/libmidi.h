#define d define
#d Type typedef struct
#d MIDI_MAX_CHANS	16	/* maximum midi channels */

/* masks */
#d M_CMD	0x80		/* midi command bit mask */
#d M_CMD_MASK	0xF0		/* midi command mask */
#d M_CHAN_MASK	0x0F		/* midi channel mask */
#d M_VAL_MASK	0x7F		/* midi value mask */

/* command ids */
/* CH_... channel commands */
#d CH_KEY_OFF	0x80		/* channel key off */
#d CH_KEY_ON	0x90		/* channel key on */
#d CH_POLY_KPRS	0xA0		/* polyphonic key pressure */
#d CH_CTL	0xB0		/* channel control */
#d CH_PRG	0xC0		/* channel program change */
#d CH_PRESSURE	0xD0		/* channel pressure */
#d CH_P_BEND	0xE0		/* channel pitch bend */

/* SX_... system exclusive commands */
#d SX_CMD	0xF0		/* system exclusive command */
#d SX_EOB	0xF7		/* system exclusive end of block */

/* SC_... system common commands */
#d SC_MSEL	0xF2		/* system common measure select */
#d SC_SSEL	0xF3		/* system common song select */
#d SC_TSEL	0xF6		/* system common tune request */

/* RT_... realtime commands */
#d RT_TCIP	0xF8		/* timing clock in play */
#d RT_TCWME	0xF9		/* timing clock w/ meas. end */
#d RT_SA1M	0xFA		/* start at first measure */
#d RT_CONT	0xFB		/* continue start */
#d RT_TCIS	0xFC		/* timing clock in stop */
#d RT_CTOH	0xFD		/* timing clock to host */
#d RT_RESET	0xFF		/* system reset */

/* default for reading/writing under regular UNIX */
#d MIDI_DEV	"/dev/ttya"
extern midi_out();
extern unsigned char midi_in(), *midi_cmd_in();

/* template for all midi parametric data */
/* note: watch out for synth. parameters that exceed u_char range! */
Type midi_par {
	unsigned char par_index;/* index in hardware table */
	unsigned char par_val;	/* current value */
	unsigned char par_ub;	/* upper boundary */
	char *par_name;		/* name */
} MidiPar;

/* same as above, but for parameters requiring u_short storage */
Type midi_spar {
	unsigned short par_index;/* index in hardware table */
	unsigned short par_val;	 /* current value */
	unsigned short par_ub;	 /* upper boundary */
	char *par_name;		 /* name */
} MidiSpar;

#d TRW_SEL_TTY	1
#d TRW_SEL_RFD	2
#d TRW_SEL_WFD	3
#undef d
