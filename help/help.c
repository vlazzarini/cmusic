#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#define L 1000
#include <stdlib.h>
main(argc,argv) int argc; char *argv[]; {
 char *sp, f[L], s[L], cmd[L], line[L], *index(), *rindex();
 struct stat statbuf;
 FILE *ls, *fp, *mo, *popen();
    if(argc == 1){
printf("__________________________________________________________\n");
printf("help is available on the topics listed below\n");
printf(" help X	prints help on topic X\n");
printf(" help -1 X	prints a 1-line summary of topic X\n");
printf(" help -1	prints a 1-line summary of ALL topics\n");
printf(" help -a X	recursively lists all subtopics of X\n");
printf(" help -a	recursively lists ALL subtopics\n");
printf(" help -s X	prints names of topics which mention X\n");
printf(" (`/' after a topic indicates that it has subtopics)\n");
printf("----------------------------------------------------------\n");
	strcpy(cmd, "ls -F -C "); strcat(cmd, HELP_DIR); system(cmd);
	exit(0);
    }
    if(argc > 1){
	if(!strcmp(argv[1], "-1")){
	    strcpy(f, HELP_DIR);
	    if(argc > 2){
		strcat(f, "/");
		strcat(f, argv[2]);
	    }
	    if( stat(f, &statbuf) == -1){
		fprintf(stderr,"help error 1 - no topic '%s'.\n",argv[2]);
		exit(-1);
	    }
	    if(statbuf.st_mode & S_IFREG){
		if( (fp = fopen(f, "r")) != NULL){
		    fgets(line, L, fp);
		    printf("%s",line);
		    fclose(fp);
		} else {
		    fprintf(stderr,"Error opening file `%s'\n",f);
		    exit(-1);
		}
		exit(0);
	    }
	    strcpy(cmd, "ls "); 
	    strcat(cmd, f);
	    ls = popen(cmd, "r");
	    mo = popen("more", "w");
	    while(fgets(s, L, ls)){
		sp = index(s, '\n');
		*sp = '\0';
		strcpy(f, HELP_DIR);
		strcat(f, "/");
		if(argc > 2){
		    strcat(f, argv[2]);
		    strcat(f, "/");
		}
		strcat(f, s);
		if( stat(f, &statbuf) == -1){
		    fprintf(stderr,"help error 2 - no topic '%s'.\n",argv[2]);
		    exit(-1);
		}
		if(statbuf.st_mode & S_IFDIR){
		    strcat(f, "/.");
		    strcat(f, s);
		}
		if( (fp = fopen(f, "r")) != NULL){
		    fgets(line, L, fp);
		    fprintf(mo, "%s", line);
		    fclose(fp);
		} else {
		    fprintf(stderr,"\nError opening file `%s'\n\n",f);
		}
	    }
	    pclose(ls);
	    pclose(mo);
	    exit(0);
	}
	if (!strcmp(argv[1], "-s")){
	    if(argc != 3){
		system(argv[0]);	/* was system(helppath) - D. Freed */
		exit(-1);
	    }

	/*
	 * All the code below was changed, since it wasn't working.
	 * -D. Freed, NUCMS, 2/8/85
	 *
	 */

	/* OLD CODE - commented out
	 *  strcpy(s, "grep -il ");
	 *  strcat(s, argv[2]);
	 *  strcat(s, " ");
	 *  strcpy(cmd, "cd ");
	 *  strcat(cmd, HELP_DIR);
	 *  strcat(cmd, "; ");
	 *  strcat(cmd, s);
	 *  strcat(cmd, "*");
	 *  system(cmd);
	 *  strcpy(cmd, "cd ");
	 *  strcat(cmd, HELP_DIR);
	 *  strcat(cmd, "; ");
	 *  strcat(cmd, "ls -R | grep : | sed -e 's:./:");
	 *  strcat(cmd, s);
	 *  strcat(cmd, ":' -e 's#:#/*#' | csh");
	 *  system(cmd);
	 */

	/* NEW CODE... */

	    strcpy(cmd, "cd ");
	    strcat(cmd, HELP_DIR);
	    strcat(cmd, "; grep -il ");
	    strcat(cmd,argv[2]);
	    strcat(cmd," `find . \\! -type d -print`  | ");
	    strcat(cmd,"sed -e '/\\/\\./d' -e 's/\\.\\///' ");
	    system(cmd);

	/* END OF NEW CODE... */

	    exit(0);
	}
	if (!strcmp(argv[1], "-a")){
	    printf("\n");
	    strcpy(s, HELP_DIR);
	    if(argc > 2){
		strcat(s, "/");
		strcat(s, argv[2]);
	    }
	    if( stat(s, &statbuf) == -1){
		fprintf(stderr,"`%s' is nonexistent\n", argv[2]);
		system(argv[0]);	/* was system(helppath) - D. Freed */
		exit(-1);
	    }
	    if(statbuf.st_mode & S_IFDIR){
		strcpy(cmd, "cd ");
		strcat(cmd, s);

		/* NOTE: the double backslashes were added below. -D. Freed */

		strcat(cmd,"; ls -F -RC | sed -e 's:\\./:");
		if(argc > 2){
		    strcat(cmd, argv[2]);
		    strcat(cmd, "/");
		}
		strcat(cmd, ":' | more");
		system(cmd);
		exit(0);
	    } else {
	fprintf(stderr,"`%s' is a topic - it has no subtopics\n", argv[2]);
		exit(-1);
	    }
	}
	strcpy(s, HELP_DIR); strcat(s, "/"); strcat(s, argv[1]);
	if( stat(s, &statbuf) == -1){
	    fprintf(stderr,"\nno help available on topic `%s'\n", argv[1]);
	    system(argv[0]);	/* was system(helppath) - D. Freed */
	    exit(-1);
	}
	if(statbuf.st_mode & S_IFDIR){
printf("__________________________________________________________\n");
printf("subtopics of supertopic %s are listed below\n", argv[1]);
printf(" help %s/X  	prints help on subtopic X\n", argv[1]);
printf(" help -1 %s/X  	prints a 1-line summary of subtopic X\n", argv[1]);
printf(" help -1 %s  	prints a 1-line summary of ALL subtopics of %s\n", argv[1],argv[1]);
printf(" help -a %s  	recursively lists all subtopics of %s\n",argv[1],argv[1]);
printf(" help -a  		recursively lists ALL subtopics\n");
printf(" (`/' after a subtopic indicates that it has further subtopics)\n");
printf("----------------------------------------------------------\n");
	    strcpy(cmd, "ls -F -C "); strcat(cmd, s); system(cmd);
	}
	if(statbuf.st_mode & S_IFREG){
/*
 * 	    printf("\n(reading file %s)\n", s);
 */
	    printf("\n");
	    strcpy(cmd, "more "); strcat(cmd, s); system(cmd);
	}

    }
}

