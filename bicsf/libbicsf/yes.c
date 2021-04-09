#include <stdio.h>

yes(question, autopilot)
	char *question; int autopilot;
{
	char ans[80];
	FILE *fopen(), *fp;
	printf("%s\t",question);
	if (autopilot) { 
		printf("y\n"); 
		return(1); 
	}
	if (fflush(stdout) < 0)
	    perror("fflush");
	fp = fopen("/dev/tty", "r");
	if (fp == NULL) {
		perror("fopen");
		return(0);
	}
	fgets(ans, 80, fp);
	if (fclose(fp) < 0)
		perror("fclose");
	return(ans[0] == 'Y' || ans[0] == 'y');
}
