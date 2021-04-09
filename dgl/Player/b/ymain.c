/* ymain.c	2.1	(CARL)	10/26/85	23:49:42 */

#include "ymain.h"
#include <stdio.h>
#include <carl/carl.h>

char *headerfile = 	"<carl/Player.h>";
int integer;

extern int yydebug;
extern int bug;
extern int dbug;
extern int m_start_seen, t_start_seen;

struct pstruct vs[256];

int playercnt = -1;
int notecalls;
int errors;

main(argc, argv) 
	int argc; char **argv;
{
	int rtn;
	int ch;

	while ((ch = crack(argc, argv, "b|i", 0)) != '\0') {
		switch (ch) {
			case 'b':
			    switch (atoi(arg_option)) {
				    case 1: 
					    yydebug = 1;
					    break;
				    case 2: 
					    bug = 1;
					    break;
				    case 3: 
					    dbug = 1;
					    break;
			    }
			    break;
			case 'i':
			    integer = 1;
			    break;
			default:
			    fprintf(stderr, "Playery: unknown flag=%c\n", ch);
			    exit(1);
		}
	}
	if (integer)
		printf("# define P_INTEGER\n");
	else
		printf("# define P_FLOAT\n");
	printf("# include %s\n", headerfile);
# ifdef notdef
	if (integer)
		printf("main()\n{\n\tPmain();\n}\n");
# endif notdef
	rtn = yyparse();
	if (bug)
		printf("\nreturning from yyparse");
	rtn = undivert(&divert_body, 
		&divert_body_cnt,
		&divert_body_len);
	printf("\n");
	setup();
	exit(errors);
}

#include <stdio.h>

extern int linecnt; 	/* from clexer.c */

yyerror(s)
	char *s;
{
	fprintf(stderr, "%s on line %d\n", s, linecnt);
	errors++;
}

setup()
{
	int i, j;

	/* make routines to initialize player instances */
	for (i = 0; i <= playercnt; i++) {
		printf("/* initialize Player instance */\n");
		printf("PI_%s(i, c)\n", vs[i].player);
		printf("\tPlayer *i, *c;\n");
		printf("{\n");
		printf("\ti->P_cproc = c->P_cproc;\n");
		printf("\ti->P_iproc = c->P_iproc;\n");
		printf("\ti->P_name = c->P_name;\n");
		printf("\ti->P_hipn = c->P_hipn;\n");
		printf("\ti->P_plen = c->P_plen;\n");
		if (vs[i].hi_ref_pn >= 0) {
			if (integer)
				printf( "\ti->P_pn=(long *) calloc(%d, (unsigned) sizeof(long));\n", vs[i].hi_ref_pn+1);
			else
				printf("\ti->P_pn=(double *)calloc(%d, (unsigned) sizeof(double));\n", vs[i].hi_ref_pn+1);
			printf("\ti->P_ps=(char **)calloc(%d, (unsigned) sizeof(char *));\n", vs[i].hi_ref_pn+1);
			printf("\ti->P_pp=(char **)calloc(%d, (unsigned) sizeof(char *));\n", vs[i].hi_ref_pn+1);
		}
		if (vs[i].has_instance_data)
			printf("\ti->P_nstnc=(char *)calloc(1, (unsigned) sizeof(struct %s));\n", vs[i].player);
		printf("}\n");
	}

	/* make routines to initialize player class templates */
	for (i = 0; i <= playercnt; i++) {
		printf("/* make Player class template */\n");
		printf("PC_%s(c)\n", vs[i].player);
		printf("\tPlayer *c;\n");
		printf("{\n");
		printf("\tc->P_cproc = %s;\n", vs[i].player);
		printf("\tc->P_iproc = PI_%s;\n", vs[i].player);
		printf("\tc->P_name = \"%s\";\n", vs[i].player);
		printf("\tc->P_hipn = c->P_plen = %d;\n", vs[i].hi_ref_pn);
		printf("\tc->P_children = %s;\n", 
			vs[i].cnt == NULL ? "0" : vs[i].cnt);
		printf("\tif (install((char *) c->P_cproc, AT_CLASS, (char *) c) == NULL)\n");
		printf("\t\tPexit(101);\n");
		printf("\tif(install(c->P_name, AT_STR_CLASS, (char *) c) == NULL)\n");
		printf("\t\tPexit(101);\n");
		printf("}\n");
	}

	/* make routines to instantiate and run static players */
	for (i = j = 0; i <= playercnt; i++) {
		if (vs[i].rstat == 0) {	/* no action times */
			printf("/* preset Player instances */\n");
			printf("PP_%s(c)\n", vs[i].player);
			printf("\tPlayer\t*c;\n");
			printf("{}\n");
			continue;	/* a player template only */
		}
		printf("/* preset Player instances */\n");
		printf("PP_%s(c)\n", vs[i].player);
		printf("\tPlayer\t*c;\n");
		printf("{\n");
		printf("\tPlayer\t*i = mk_Player(c);\n");
		printf("\tPI_%s(i, c);\n", vs[i].player);
		if (integer) {
			printf("\ti->P_beg\t= (long) %s;\n", vs[i].begin);
			printf("\ti->P_time\t= (long) %s;\n", vs[i].begin);
			printf("\ti->P_dur\t= (long) %s;\n", vs[i].dur);
		} else {
			printf("\ti->P_beg\t= (double) %s;\n", vs[i].begin);
			printf("\ti->P_time\t= (double) %s;\n", vs[i].begin);
			printf("\ti->P_dur\t= (double) %s;\n", vs[i].dur);
		}
		if (vs[i].stat != NULL)
			printf("\ti->P_runstat\t= %s;\n", vs[i].stat);
		else
			printf("\ti->P_runstat\t= %s;\n", "P_READY");
		printf("\tset_wait(i, i->P_time);\n");
		printf("}\n");
		j++;
	}
}
