/* For commands locking */

#define MAXLOCKDEVS 8
#define DISKLOCK_ADD 01
#define DISKLOCK_REMOVE 02
#define DISKLOCK_LOCK 03
#define DISKLOCK_UNLOCK 04
#define DISKLOCK_SETGROUP 05

/* Values in d_bdevs */
#define DISKLOCK_ENABLED 01
#define DISKLOCK_DISABLED 00

/* Value in upids for not locked else it is pid who has lock */
#define DISKLOCK_NOTLOCKED (-1)
/* Group not set */
#define DISKLOCK_NOGROUP (-1)

/* Lockable holders */
struct disklock {
	char d_bdevs[MAXLOCKDEVS]; 	/* Units on this dev */
	short d_upids[MAXLOCKDEVS] ;
};

#ifdef KERNEL
extern struct disklock bdisklocks[],cdisklocks[];
extern int lockdebug,disklock_group;
#endif

