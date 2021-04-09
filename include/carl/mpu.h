/* %M%	%I%	(CARL)	%G%	%U% */

typedef struct mpu_cmd {
	unsigned char time_tag;
	unsigned char arg_cnt;
	unsigned char cmd_cont;
	unsigned char cmd_type;
	long mpu_time;
	unsigned char *mpu_cmd;
} MpuCmd;
extern MpuCmd *GetMpuCmd();
extern struct   mpu_cmd *get_mpu_cmd();


#define MpuPitch(m)	   (m->mpu_cmd[1]) /* return pitch field of MpuCmd */
#define MpuVelocity(m)  (m->mpu_cmd[2]) /* return velocity field of MpuCmd */
#define MPU_DEV_0		"/dev/mpu0"
#define MidiDevice		MPU_DEV_0

/* Following is taken from the MPU technical reference manual */
#define MPU_CLOCK_PERIOD 		(240)
#define MPU_DEFAULT_TIME_BASE	(120)
#define MPU_DEFAULT_TEMPO		(60.0)	/* beats/minute */
#define MPU_TPS(tempo, time_base)	(((time_base)*tempo)/60.0)
#define MPU_DEFAULT_TICS_PER_SEC	MPU_TPS(MPU_DEFAULT_TEMPO,MPU_DEFAULT_TIME_BASE)
#define MPU_STOP_PLAY		(0x05)
#define MPU_START_PLAY		(0x0a)
#define MPU_CONTINUE_PLAY		(0x0b)
#define MPU_STOP_RECORD		(0x11)
#define MPU_STOP_OVERDUB		(0x15)
#define MPU_RECORD_STANDBY		(0x20)
#define MPU_START_RECORD		(0x22)
#define MPU_START_OVERDUB		(0x2a)
#define MPU_ALL_NOTE_OFF		(0x30)	/* turn all notes off */
#define MPU_NO_REAL_TIME		(0x32)	/* do not send real time messages to MIDI OUT */
#define MPU_THRU_OFF			(0x33)	/* disable THRU function */
#define MPU_TIMING_ON		(0x34)
#define MPU_MODE_MESS_ON		(0x35)	/* let host see mode msgs , to act as a sound generator*/
#define MPU_THRU_ACTIVE_SENSING	(0x36)
#define MPU_THRU_EXCLU_ON		(0x37)	/* allow system-exclusive msgs from MIDI in->out */
#define MPU_COMMON_TO_HOST_ON	(0x38)	/* tell mpu to send system-common msgs to host */
#define MPU_REAL_TIME_TO_HOST_ON	(0x39)	/* tell mpu to send clock time to host */
#define MPU_UART_MODE		(0x3f)	/* set mpu to simple uart-mode; MPU_RESET ends */
#define MPU_DATA_END			(0xfc)	/* */
#define MPU_INT_CLOCK		(0x80)	/* allow use of mpu clock (cf SET_TEMPO, TIMEBASE) */
#define MPU_FSK_CLOCK		(0x81)	/* mpu syncs clock with TAPE IN (for tape playback) */
#define MPU_MIDI_CLOCK		(0x82)	/* mpu syncs clock with MIDI IN */
#define MPU_METRO_NO_ACC		(0x83)	/* turn mpu metronome on, no accent */
#define MPU_METRO_OFF		(0x84)	/* turn mpu metronome off */
#define MPU_METRO_ACC		(0x85)	/* turn mpu metronome on, with accent */
#define MPU_BENDER_OFF		(0x86)	/* disable flood of continuous control messages to host */
#define MPU_BENDER_ON		(0x87)	/* enable continuous control messages */
#define MPU_MIDI_THRU_OFF		(0x88)	/* disable MPU MIDI IN -> MIDI OUT */
#define MPU_MIDI_THRU_ON		(0x89)	/* enable MPU MIDI IN->OUT */
#define MPU_DATA_STOP_RECORD_OFF	(0x8a)
#define MPU_DATA_STOP_RECORD_ON	(0x8b)
#define MPU_SEND_MEASURE_END_OFF	(0x8c)
#define MPU_SEND_MEASURE_END_ON	(0x8d)
#define MPU_CONDUCTOR_ON		(0x8e)	/* enable CONDUCTOR commands while in play mode */
#define MPU_CONDUCTOR_OFF		(0x8f)
#define MPU_FSK_TO_INT		(0x92)	/* set tape sync clock resolution to internal TIMEBASE */
#define MPU_FSK_TO_MIDI		(0x93)	/* set MIDI sync clock resolution to internal TIMEBASE */
#define MPU_CLOCK_TO_HOST_OFF	(0x94)	/* do not send clock data to host */
#define MPU_CLOCK_TO_HOST_ON		(0x95)	/* send clock data to host */
#define MPU_EXCLUSIVE_TO_HOST_OFF	(0x96)
#define MPU_EXCLUSIVE_TO_HOST_ON	(0x97)
#define MPU_RESET_TEMPO		(0xb1)	/* reset tempo to 1/1 */
#define MPU_CLEAR_PLAY_COUNTERS	(0xb8)	/* clear all play counters (must do before START_PLAY) */
#define MPU_TIMEBASE_48		(0xc2)	/* set timebase to 48 */
#define MPU_TIMEBASE_72		(0xc3)	/* set timebase to 72 */
#define MPU_TIMEBASE_96		(0xc4)	/* set timebase to 96 */
#define MPU_TIMEBASE_120		(0xc5)	/* set timebase to 120 (recommended timebase) */
#define MPU_TIMEBASE_144		(0xc6)	/* set timebase to 144 */
#define MPU_TIMEBASE_168		(0xc7)	/* set timebase to 168 */
#define MPU_TIMEBASE_192		(0xc8)	/* set timebase to 192 */
#define MPU_WANT_TO_SEND_DATA	(0xd0)	/* send un-timetagged midi data */
#define MPU_SEND_SYSTEM_MESSAGE	(0xdf)	/* allow host to transmit system exclusive messages */
#define MPU_TEMPO			(0xe0)	/* set tempo (beats/minute) to following byte */
#define MPU_RELATIVE_TEMPO		(0xe1)	/* set timing rate of change (e.g., for ritardando) */
#define MPU_GRADUATION		(0xe2)	/* set RELATIVE_TEMPO (0=immediate,1=slow,0xff=fast */
#define MPU_MIDI_METRO		(0xe4)	/* set # of midi tics/beep (24 => beep every 1/4 note */
#define MPU_METRO_MEAS		(0xe6)	/* set beats/measure; if mm = 1/4 note, 4 => 4b/m */
#define MPU_SET_INT_CLOCK		(0xe7)	/* set rate of CLOCK_TO_HOST (eg to count beats on host)*/
#define MPU_ACTIVE_TRACKS		(0xec)	/* enable/disable tracks 1-8 */
#define MPU_CHANNELS_1to8		(0xee)	/* enable/disable channels 1-8 during RECORD */
#define MPU_CHANNELS_9to16		(0xef)	/* enable/disable channels 9-16 during RECORD */
#define MPU_RESET			(0xff)
