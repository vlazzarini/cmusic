/* Psleep.c	2.2	(CARL)	1/30/86	16:32:41 */

# include <setjmp.h>
# include <signal.h>
# include <sys/time.h>
# include <math.h>

static jmp_buf	jbuf;

Psleep(interval)
	float interval;
/*
 * Floating point sleep routine for 4.2BSD
 * .LP
 * Takes time in real seconds (including fractional time),
 * precise down to microseconds, accurate only to system
 * clock rate.
 * If 'interval == 0', the timer is disabled, and Psleep hangs.
 */
{
	extern void catchalarm();
	struct itimerval val, oval;
	register long tmp;

	signal(SIGALRM, catchalarm);

	if (setjmp(jbuf))
		return;

	val.it_interval.tv_sec = 0;
	val.it_interval.tv_usec = 0;
	val.it_value.tv_sec = tmp = interval;
	val.it_value.tv_usec = (interval - tmp) * 1000000.0;

	setitimer(ITIMER_REAL, &val, &oval);

	pause();
}

void
catchalarm() 
{
	longjmp(jbuf, 1);
}

/*
 * main(argc, argv)
 * 	char **argv;
 * {
 * 	extern char *ctime();
 * 	struct timeval timeval; 
 * 	struct timezone timezone;
 * 	printf("!\n");
 * 	if (argc > 1) {
 * 		gettimeofday(&timeval, &timezone);
 * 		printf(ctime(&timeval.tv_sec));
 * 		Psleep(atof(argv[1]));
 * 		gettimeofday(&timeval, &timezone);
 * 		printf(ctime(&timeval.tv_sec));
 * 	}
 * }
 */
