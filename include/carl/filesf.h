/* %M%	%I%	(CARL)	%G%	%U% */

/* Name of the disk cylinder allocation file */
#define ALLOCNAME "/dskcyls"
/* temp name used in updating ALLOCNAME */
#define SAVALLOC "/dskcyls.bak"

/* default path head for /snd directory */
#define SDFDEV "/snd"
/* number of slashes to search for in removing SFDEV from a name */
#define SFDEVSLSH 2
/* default sound file name body */
#define SDFSFN "/test"
/* extension for sound file descriptors */
#define SDFEXT ".sdf" 
#define SNDEXT ".snd"
#define SFENVDEV "SFDEV"	/* environment variable that sets def. dev. */
#define SFENVPATH "SFPATH"	/* environment variable that sets def. path */
#define SFENVNAME "SFNAME"	/* env. var. that sets def. file body */
#define PHANTOM "phantom"
/* unix group that has super-user status for fixing sound file system */
#define SUPERGROUP "disk"
/* person to inform for errors */
#define SNDSUPERUSER "disk"
#define BADBLOCKFILE "BADBLOCK"
/* name of regular unix files used to schedule access to csound system */
#define SF_LOCK_FILE "/sflock"
#define SF_LOCK_LINKTO "/sflink"
/* existence of this file freezes all csound i/o, save for superusers */
#define MASTERLOCK "/MASTERLOCK"
#define	SFINCREM "/sfdumpinfo"
#define SFTMPINCREM "/tmpsfdump"
