/*
 * command line:
 * sched [flags] [day] [from] [to] < schedule
 *
 * schedule entry format:
 * name
 * day_of_week [from_hour to_hour] ...
 * ...
 * name
 * day_of_week [from_hour to_hour] ...
 * ...
 */

#define SCHEDLEN 512

struct sched {
	char week[7][24];
	char name[32];
	char count;
} sh[SCHEDLEN];

struct sched com;

#define NAME 0
#define DAY 1
#define FROM 2
#define TO 3

#define PSCHED  01
#define COMMON  02
#define SUBTIME 04

int flags = COMMON;
char target[32];

extern int arg_index;		/* from crack.c */
extern char *arg_option;	/* from crack.c */
extern char crack();

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

char *pday(int day)
{
	char *name;
	switch (day) {
		case 0: return("mon");
		case 1: return("tue");
		case 2: return("wed");
		case 3: return("thu");
		case 4: return("fri");
		case 5: return("sat");
		case 6: return("sun");
	}
}

void common(int len)
{
	int day, hour;
	printf("number of participants: %d, common schedule:\n", len);
	printf("%s\n", 
		"\t                         1 1 1 1 1 1 1 1 1 1 2 2 2 2");
	printf("%s\n", 
		"\t     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3");
	printf("%s\n", 
		"\t     - - - - - - - - - - - - - - - - - - - - - - - -");
	for (day = 0; day < 7; day++) {
		printf("\t%s:", pday(day));
		for (hour = 0; hour < 24; hour++) {
			if (com.week[day][hour] == len)
				printf(" *");
			else
			if (com.week[day][hour] == 0)
				printf(" |");
			else
				printf("%2d", com.week[day][hour]);
		}
		printf("\n");
	}
}


void
subtime(int len, int day, int from, int to)
{
	int i, j, hours = to - from;
	for (i = 0; i < len; i++) {
		for (j = from; j < to; j++) {
			if (sh[i].week[day][j] != 0) {
				sh[i].count++;
			}
		}
	}
	for (i = 0; i < len; i++) {
		if (sh[i].count > 0) {
			printf("%s\t\t", sh[i].name);
			printf("%d hours out of %d\n", sh[i].count, hours);
		}
	}
}


void
psched(len)
	int len;
{
	int who, day, hour;
	
	
	for (who = 0; who < len; who++) {
		if (*target != '\0' && strcmp(target, sh[who].name))
			continue;
		printf("%s\n", sh[who].name);
		printf("%s\n", "\t    000000000011111111112222");
		printf("%s\n", "\t    012345678901234567890123");
		for (day = 0; day < 7; day++) {
			printf("\t%s:", pday(day));
			for (hour = 0; hour < 24; hour++)
				printf("%c", 
					sh[who].week[day][hour] ? '*' : ' ');
			printf("\n");
		}
	}
}


main(argc, argv)
	char **argv;
{
	int who = -1, seq = 0, day, from, to, i, j = 0, line = 0;
	int cday, cfrom = 0, cto = 24;
	char field[128], ch;

	while ((ch = crack(argc, argv, "p|", 0)) != NULL) 
	    {
	    switch (ch) 
		    {
		    case 'p':	flags |= PSCHED; 
				flags &= ~COMMON;
				strcpy(target, arg_option);
				break;
		    }
	    }
	for (i = arg_index; i < argc; i++) {
		switch (j++) {
			case 0: cday = dayconv(argv[j]); break;
			case 1: cfrom = atoi(argv[j]); break;
			case 2: cto = atoi(argv[j]); 
		}
		flags &= ~COMMON; /* so who cares if we do it thrice? */
		flags |= SUBTIME;
	}
	
	while (scanf("%s", field) != EOF) {
		if (!strcmp(field, "/*")) {
			do {
				if (scanf("%s", field) == EOF)
					exit(1);
			    } while (strcmp(field, "*/"));
			continue;
		}
again:		switch (seq) {
			case NAME:
				if (who >= SCHEDLEN) {
					fprintf(stderr, 
						"too many participants!\n");
					exit(1);
				}
				strcpy(sh[++who].name, field);
				seq++;
				line++;
				break;
			case DAY:
				if (isdigit(*field)) {
					fprintf(stderr, 
						"day expected on line %d\n",
						line);
					exit(1);
				}
				if ((day = dayconv(field)) < 0) {
					seq--;
					goto again;
				}
				seq++;
				line++;
				break;
			case FROM:
				if (!isdigit(*field)) {
					seq--;
					goto again;
				}
				from = atoi(field);
				seq++;
				break;
			case TO:
				to = atoi(field);
				if (to < from) {
					fprintf(stderr, 
					    "range format error on line %d\n",
					    line);
					exit(1);
				}
				for (j = from; j < to; j++) {
					sh[who].week[day][j] = 1;
					com.week[day][j]++;
				}
				seq--;
				break;
		}
	}
	if (flags & PSCHED) psched(who+1);
	if (flags & COMMON) common(who+1);
	if (flags & SUBTIME) subtime(who+1, cday, cfrom, cto);
}

int
dayconv(day)
	char *day;
{
	
	int i;
	if (isupper(*day)) 
		*day += 'a' - 'A';
	for (i = 0; i < 7; i++) {
		if (!strncmp(day, pday(i), 3))
			return(i);
	}
	return(-1);
}



	


