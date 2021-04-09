/* secret France-USA relationships		*/

#define mpu_xc_write(mpu,data)	mpuwrite(data,mpu)
#define mpu_xc_read(mpu)		mpuread(mpu)
#define mpu_xc_cmd(mpu,cmd)	mpuwcmd(cmd,mpu)

#define NULL 0

#define MPUIT	(int *)0x6c	/* level 3 intr	autovecteur	*/
#define MPUDATA	(char *)0xff0100	/* data register mpu	*/
#define MPUCMD	(char *)0xff0101	/* mpu status and cmommand register*/

#define MPU0_DATA	0xff0100
#define MPU0_CMD	0xff0101

#define MPU_NB	4	/* nb max MPU			*/
# define TRACK_NB	8	/* 8 tracks	*/

# define u_char unsigned char

/* type for address table for the MPU	*/
typedef struct {
	char *mpu_reg_data ;	/* data register	*/
	char *mpu_reg_cmd;	/* status and command register	*/
} MPU_REG ;

/* macros for MPU	*/

/* non blocking check status, read and write		*/
#define m_mpuwcmd(cmd)	*MPUCMD = (char)cmd
#define m_mpustatus()	(char)(*MPUCMD)

#define m_mpumwrite(data)	*MPUDATA = (char)data
#define m_mpumread()	(char)(*MPUDATA)


/*	init	*/
#define	m_mpureset(no)	mpuwcmd(0xff,no)
#define	m_initthruoff(no)	mpuwcmd(0x33,no)

/* 	clear		*/
#define	m_clrplay(no)		mpuwcmd(0xb8, no)
#define	m_clrrec(no)		mpuwcmd(0xba, no)
#define	m_clrtable(no)		mpuwcmd(0xb9, no)

/*	swithes		*/
#define	m_thruon(no)	mpuwcmd(0x89, no)
#define	m_thruoff(no)	mpuwcmd(0x88, no)

#define	m_metroon(no)	mpuwcmd(0x83,no)
#define m_metrooff(no)	mpuwcmd(0x84, no)
#define m_metroacc(no)	mpuwcmd(0x85, no)

#define m_bendon(no)	mpuwcmd(0x87, no)
#define m_bendoff(no)	mpuwcmd(0x86, no)

/*	record	*/
#define	m_startrec(no)	mpuwcmd(0x23, no)
#define	m_stoprec(no)	mpuwcmd(0x11, no)

/*	play	*/
#define	m_startplay(no)	mpuwcmd(0x0a, no)
#define	m_stopplay(no)	mpuwcmd(0x05, no)
#define m_contplay(no )	mpuwcmd(0x0b, no)

/*	overdub	*/
#define	m_startovd(no)	mpuwcmd(0x2b, no )
#define	m_stopovd(no )	mpuwcmd(0x15, no)

/*	measure end	*/
#define m_measoff(no)	mpuwcmd(0x8c, no )
#define m_meason(no)	mpuwcmd(0x8d, no)


/* constante pour les pedales		*/

#define P_REPOS	0xff
#define P_ON	0x7f
#define P_OFF	0
