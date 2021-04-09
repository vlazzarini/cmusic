/* dx7.h	1.1	(CARL)	6/3/85	10:34:52 */

/* manufacturer's midi id number */
#define	ID_DX7			(67)	/* Yamaha DX7 */

#define DX7VOXLEN		(155)	/* length of dx7 single voice table */
#define DX732VOXLEN		(128)	/* length of dx7 32-voice table */
#define DX7COMLEN		(4096)	/* length of dx7 common voice table */
#define DX7FUNLEN		(14)	/* length of dx7 function table */
#define DX7CTLLEN		(127)	/* length of dx7 control table */
#define DX7KBDLEN		(127)	/* length of dx7 keyboard table */
#define DX7NVOX			(32)	/* number of dx7 voices */
#define DX7NVOICES		(16)	/* 16 simultaneous voices in poly mode*/
#define	DX7OPLEN		(21)	/* length of dx7 operator table */
#define	DX732VOPLEN		(17)	/* length of 32-voice operator table */
#define DX7NOPS			(6)	/* number of dx7 operators per voice */
#define DX7OPSLEN		(DX7OPLEN * DX7NOPS)	/* total len of op tbl*/
#define DX732VOPSLEN		(DX732VOPLEN * DX7NOPS)	/* 32-voice op tbl len*/

/* SX == dx7 system exclusive command or field */
#define	SX_DX7_1V		(0)	/* dx7 1 voice format */
#define	SX_DX7_32V		(9)	/* dx7 32 voice format */
#define	SX_DX7_BD		(0)	/* dx7 bulk data sub status */
#define	SX_DX7_PC		(1)	/* dx7 parameter change sub status */
#define SX_DX7_AS		(2)	/* dx7 pseudo sub status active sense */
#define SX_DX7_GRP_VPAR		(0)	/* dx7 voice parameter group */
#define SX_DX7_GRP_FPAR		(2)	/* dx7 function parameter group */

/* CTL == dx7 channel control command indicies */
#define	CTL_DX7_MOD_WHEEL	(1)	/* dx7 chan mod wheel index */
#define	CTL_DX7_BREATH		(2)	/* dx7 chan breath index */
#define	CTL_DX7_AFTER		(3)	/* dx7 chan after touch index */
#define	CTL_DX7_FCONT		(4)	/* dx7 chan foot control index */
#define CTL_DX7_PORTA_TIME	(5)	/* dx7 chan portamento index */
#define	CTL_DX7_DE_KNOB		(6)	/* dx7 chan data entry knob index */
#define	CTL_DX7_VOLUME		(7)	/* dx7 chan volume control index */
#define	CTL_DX7_SUST_FSW	(64)	/* dx7 chan footswitch control index */
#define	CTL_DX7_PORTA_FSW	(65)	/* dx7 chan portamento control index */
#define	CTL_DX7_DE_PLUS		(96)	/* dx7 chan data entry + sw. index */
#define	CTL_DX7_DE_MINUS	(97)	/* dx7 chan data entry - sw. index */
#define CTL_DX7_OMNI_AKO	(125)	/* dx7 chan omni all keys off index */
#define CTL_DX7_MONO_AKO	(126)	/* dx7 chan mono all keys off index */
#define CTL_DX7_POLY_AKO	(127)	/* dx7 chan poly all keys off index */

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

/* one dx7 contains storage for all the following */
struct dx7 {
	u_char	dx7_vox_val[DX7NVOX][DX7VOXLEN];	/* voice data */
	u_char dx7_fun_val[DX7FUNLEN];			/* function data */
	u_char dx7_kbd_val[DX7CTLLEN];			/* keyboard data */
	u_char dx7_ctl_val[DX7CTLLEN];			/* controller data */
	u_char	dx7_nsv;				/* voices on */
	u_char	dx7_prg_val;				/* program */
	u_short	dx7_pbd_val;				/* pitch bender */
};

/* DX7_... == macros to stuff dx7 structure */
/* set voice parameter */
#define DX7_VP(dx, voice, param) 	((dx).dx7_vox_val[voice][param])
/* set function parameter */
#define DX7_FP(dx, param) 		((dx).dx7_fun_val[param])
/* set control parameter */
#define DX7_CP(dx, param) 		((dx).dx7_ctl_val[param])
/* set keyboard velocity parameter */
#define DX7_KP(dx, param) 		((dx).dx7_kbd_val[param])
/* set program parameter */
#define DX7_PP(dx) 			((dx).dx7_prg_val)
/* set pitch bend parameter */
#define DX7_BP(dx) 			((dx).dx7_pbd_val)

/* 
 * translate from common voice parameter index in the range of
 * 0 to DX7VOXLEN to ini_... array field 
 */
#define DX7_INI_IND(i) \
	(((i) < DX7OPSLEN) ? ((i) % DX7OPLEN) : (((i) % DX7OPSLEN) + DX7OPLEN))

/* 
 * set voice parameter with bounds checking - expression evaluates to -1
 * if val < 0 || val > upper bound for that parameter
 */
#define DX7_SET_VP(dx, voice, param, val) 	\
    (((dx).dx7_vox_val[voice][param]) =		\
	((val) < 0 || (val) > dx7_ini_vox[DX7_INI_IND(param)].par_ub) ? \
	    -1 : (val))

/* dx7 voice operator parameter symbolic names */
/* operator rate amplitude envelope array */
#define OP_AMP_RATE_BASE		0
#define OP_AMP_RATE_LEN			4
#define	op_amp_rate(i)			(OP_AMP_RATE_BASE + i)
/* operator level amplitude envelope array */
#define OP_AMP_LEVEL_BASE		4
#define OP_AMP_LEVEL_LEN		4
#define	op_amp_level(i)			(OP_AMP_LEVEL_BASE + i)
#define	op_kls_breakpoint		8	/* keyboard level sense */
#define	op_kls_left_depth		9
#define	op_kls_right_depth		10
#define	op_kls_left_curve		11
#define	op_kls_right_curve		12
#define	op_kbd_rate_sc			13	/* keyboard rate scale */
#define	op_mod_sens_amp			14	/* amp. mod. sensitivity */
#define	op_key_vel_sens			15	/* key velocity sensitivity */
#define	op_output_level			16	/* output level */
#define	op_osc_mode			17	/* osc. mode */
#define	op_osc_frq_coarse		18	/* osc. coarse freq. */
#define	op_osc_frq_fine			19	/* osc. fine freq. */
#define	op_detune			20	/* osc. detune */

/* OP == set operator in voice */
/* Note: for each voice, operator table goes from 5 -> 0 */
/* set operator n to parameter p */
#define OP(n, p)			((DX7NOPS - n) * DX7OPLEN + p)

/* voice parameters other than operators */
#define VOX_PITCH_RATE_BASE		126
#define VOX_PITCH_RATE_LEN		4
#define	vox_pitch_rate(i)		(VOX_PITCH_RATE_BASE + i)
#define VOX_PITCH_LEVEL_BASE		130
#define VOX_PITCH_LEVEL_LEN		4
#define	vox_pitch_level(i)		(VOX_PITCH_LEVEL_BASE + i)
#define	vox_alg				134	/* algorithm select */
#define	vox_feedback			135	/* oscillator feedback ena. */
#define	vox_osc_sync			136	/* osc. sync. */
#define	lfo_speed			137	/* lfo speed */
#define	lfo_delay			138	/* lfo delay */
#define	lfo_pmd				139	/* lfo pitch mod. depth */
#define	lfo_amd				140	/* lfo amplitude mod. depth */
#define	lfo_sync			141	/* lfo sync. ena. */
#define	lfo_wave			142	/* lfo waveform select */
#define	vox_mod_sens_pitch		143	/* mod. sens. pitch */
#define	vox_transpose			144	/* transpose oscillator */
#define VOX_NAME_BASE			145
#define VOX_32V_NAME_BASE		118
#define VOX_NAME_LEN			10
#define	vox_voice_name			VOX_NAME_BASE
#define	vox_op_ena			155	/* operator enable table */

/* function parameters */
#define	fun_mono_poly			0	/* mono/poly mode change */
#define	fun_pitch_bend_range		1	/* pitch bend range */
#define	fun_pitch_bend_step		2	/* pitch bend step */
#define	fun_porta_mode			3	/* portamento ena. */
#define	fun_porta_gliss			4	/* portamento glissando ena. */
#define	fun_porta_time			5	/* portamento time */
#define	fun_mod_wheel_range		6	/* mod. wheel range */
#define	fun_mod_wheel_assign		7	/* mod. wheel assign */
#define	fun_foot_range			8	/* foot controller range */
#define	fun_foot_assign			9	/* foot controller asign */
#define	fun_breath_range		10	/* breath controller range */
#define	fun_breath_assign		11	/* breath controller assign */
#define	fun_after_touch_range		12	/* after touch range */
#define	fun_after_touch_assign		13	/* after touch assign */

/* copy voice name field into dx7 structure */
#define	DX7_SET_VNAME(dx, voice, name) \
	(strncpy(&DX7_VP(dx, voice, vox_voice_name), name, VOX_NAME_LEN))

/* get voice name address */
#define DX7_VNAME_ADDR(dx, voice) (&(dx).dx7_vox_val[voice][VOX_NAME_BASE])

/* copy voice name field out of dx7 structure */
#define DX7_GET_VNAME(dx, buf, voice) \
	(strncpy(buf, DX7_VNAME_ADDR(dx, voice), VOX_NAME_LEN))

/* 
 * translate from 32-voice parameter index in the range of
 * 0 to DX7VOXLEN to 32v_... array field 
 */
#define DX7_32V_IND(i) \
	(((i) < DX732VOPSLEN) ? ((i) % DX732VOPLEN) : (((i) % DX732VOPSLEN) + DX732VOPLEN))
