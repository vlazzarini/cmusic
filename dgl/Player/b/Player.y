/* %M%	%I%	(CARL)	%G%	%U% */

%{
int bug;
int dbug;
int crlycnt;
int inplayer, first_inst;
int parencnt, countparens;
int *getout();
int m_start_seen, t_start_seen;
long foo;
char *scpy();
char *getpnum();
char white[];
#define YYDEBUG
#include <stdio.h>
#include <ctype.h>
#include "ymain.h"
%}
%union {
	int i;
	char *s;
	}
%start start
%type <s> start stat tstat
%type <s> obj  args arg_obj cycl player cyclh gen genh motive motivh
%token <s> LSTART LPERIOD LINT LFLOAT LWHITE LSYMBOL LCOMMA 
%token <s> LOCTAL LHEX LQSTR LBRACKETS LOP LQCHR LARB LSEMI 
%token <s> LCOMMENT LERROR LLPAREN LRPAREN LLCRLY LRCRLY LLSQB LRSQB
%token <s> PLAYER EXTERN PNFIELD PIFIELD PSFIELD PPFIELD CYCLIST V_ST_REF GEN
%token <s> P_START M_START M_TER INSTANCE MOTIVE
%%
start	: 
		{ $$ = NULL; }
	|  error LSEMI
		{}
	|  start stat 
		{
		if(bug)printf("%s$%s$\n----------\n", 
			"start 1: start stat=\n",
			$2);
		foo = divert(&divert_body, &divert_body_cnt,
			&divert_body_len, $2);
		free($2);
		}
	|  start tstat 
		{
		if(bug)printf("%s$%s$\n----------\n", 
			"start 2: start tstat=\n",
			$2);
		foo = divert(&divert_body, &divert_body_cnt,
			&divert_body_len, $2);
		free($2);
		}
	|  start INSTANCE tstat 
		{
		if(bug)printf("%s$%s$%s$\n----------\n", 
			"start 2a: start INSTANCE tstat=\n",
			$2, $3);
		if (!inplayer)
			yyerror("Instance declaration not in body of Player");
		if (first_inst == 0) {
			char *x = scpy(3, "struct ", vs[playercnt].player,
				" {\n");
			foo = divert(&divert_instance_decl, 
				&divert_instance_decl_cnt,
				&divert_instance_decl_len, x);
			first_inst = 1;
			vs[playercnt].has_instance_data = 1;
			free(x);
		}
		foo = divert(&divert_instance_decl, &divert_instance_decl_cnt,
			&divert_instance_decl_len, $3);
		parse_decl($3);
		free($2);
		free($3);
		}
	|  start LRCRLY 
		{
		if(bug)printf("%s$%s$\n----------\n",
			"start 3: start LRCRLY=\n", 
			$2);
		if (--crlycnt == 0 && inplayer) {
			if (first_inst != 0) {
				char *x = "\n};\n";
				foo = divert(&divert_instance_decl, 
					&divert_instance_decl_cnt,
					&divert_instance_decl_len, x);
			}
			foo = undivert(&divert_instance_decl,
				&divert_instance_decl_cnt,
				&divert_instance_decl_len);
			foo = undivert(&divert_body, 
				&divert_body_cnt,
				&divert_body_len);
			printf("%s%s%s%s", 
				white, 
				"\treturn(*_x);\n",
				white, 
				$2); 
			inplayer = 0;
			first_inst = 0;
			clear_hash();
		} else {
			char *x = scpy(3, white, $2, "\n");
			foo = divert(&divert_body, &divert_body_cnt,
				&divert_body_len, x);
			free(x);
		}
		if (crlycnt < 0)
		    yyerror("Playery: more right than left curly braces seen");
		free($2);
		}
	|  start player LSYMBOL LLPAREN args LRPAREN 
		{
		if(bug)printf("%s$%s$%s$%s$%s$%s$\n----------\n",
			"start 4: start player LSYMBOL LLPAREN args LRPAREN=\n",
			$2, $3, $4, $5, $6);
		foo = undivert(&divert_body, &divert_body_cnt,
			&divert_body_len);
		{
			char *x = scpy(7, $2, "Player\n",
				$3, $4, "_x", $6, "\n\tPlayer *_x;");
			foo = divert(&divert_body, &divert_body_cnt,
				&divert_body_len, x);
			free(x);
		}
		vs[playercnt].player = $3;
		if ($5 != NULL) {			/* any args? */
			char *x, *y, *z, *index();	/* yes */
			int	flag = 1;

			/* how many args? */
			if ((x = index($5, ',')) != NULL) {
				flag++;	/* 2 args */
				*x = '\0';
				if ((y = index(x+1, ',')) != NULL) {
					flag++;	/* 3 args */
					*y = '\0';
					if ((z = index(y+1, ',')) != NULL) {
						flag++;	/* 4 args */
						*z = '\0';
					}
				}
			}

			switch (flag) {
			case 1:
				vs[playercnt].begin = scpy(1, $5);
				if (integer)
				    vs[playercnt].dur = scpy(1, "-1L");
				else
				    vs[playercnt].dur = scpy(1, "-1.0");
				vs[playercnt].stat = scpy(1, "P_READY");
				vs[playercnt].cnt = scpy(1, "1");
				break;
			case 2:
				vs[playercnt].begin = scpy(1, $5);
				vs[playercnt].dur = scpy(1, x+1);
				vs[playercnt].stat = scpy(1, "P_READY");
				vs[playercnt].cnt = scpy(1, "1");
				break;
			case 3:
				vs[playercnt].begin = scpy(1, $5);
				vs[playercnt].dur = scpy(1, x+1);
				vs[playercnt].stat = scpy(1, y+1);
				vs[playercnt].cnt = scpy(1, "1");
				break;
			case 4:
				vs[playercnt].begin = scpy(1, $5);
				vs[playercnt].dur = scpy(1, x+1);
				vs[playercnt].stat = scpy(1, y+1);
				vs[playercnt].cnt = scpy(1, z+1);
				break;
			}
			vs[playercnt].rstat = 1;
		} else {			/* no args */
			vs[playercnt].rstat = 0;
		}
		countparens = 0;
		inplayer = 1;
		free($2); free($4); 
		if ($5 != NULL) 
			free($5); 
		free($6);
		}
	;


player	:  PLAYER
		{
		$$ = scpy(1, white);
		free($1);
		countparens = 1;
		parencnt = 0;
		playercnt++;
		}
	;

args	:
		{ 
		if(bug)printf("arg 0: <NULL>\n----------\n");
		$$ = NULL; 
		}
	|  arg_obj
		{
		if(bug)printf("%s$%s$\n----------\n",
			"arg 1: arg_obj=\n",
			$1);
		$$ = scpy(1, $1);
		free($1); 
		}
	|  args arg_obj
		{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"arg3: args arg_obj=\n",
			$1, $2);
		$$ = scpy(2, $1, $2);
		free($2);
		}
	|  args LCOMMA arg_obj
		{
		if(bug)printf("%s$%s$%s$%s$\n----------\n",
			"arg4: args LCOMMA arg_obj=\n",
			$1, $2, $3);
		$$ = scpy(3, $1, $2, $3);
		free($2); free($3);
		}
	;

arg_obj :  LSYMBOL
		{ $$ = scpy(2, white, $1); free($1); }
	|  LINT 
		{ $$ = scpy(2, white, $1); free($1); }
	|  LFLOAT
		{ $$ = scpy(2, white, $1); free($1); }
	|  LOCTAL
		{ $$ = scpy(2, white, $1); free($1); }
	|  LHEX
		{ $$ = scpy(2, white, $1); free($1); }
	|  LARB
		{ $$ = scpy(2, white, $1); free($1); }
	|  LLPAREN
		{ $$ = scpy(2, white, $1); free($1); }
	|  LLSQB
		{ $$ = scpy(2, white, $1); free($1); }
	|  LRSQB
		{ $$ = scpy(2, white, $1); free($1); }
	|  LOP
		{ $$ = scpy(2, white, $1); free($1); }
	;

stat	:  obj 
		{
		if(bug)printf("%s$%s$\n----------\n",
			"stat 0: obj=\n",
			$1);
		$$ = scpy(1, $1);
		free($1); 
		} 
	|  stat obj 
		{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"stat 1: stat obj=\n",
			$1, $2);
		$$ = scpy(2, $1, $2);
		free($1); free($2);
		}
	|  stat cycl 
		{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"stat2: stat cycl=\n",
			$1, $2);
		$$ = scpy(2, $1, $2);
		free($1); free($2);
		}
	|  stat gen 
		{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"stat 3: stat gen=\n",
			$1, $2);
		$$ = scpy(2, $1, $2);
		free($1); free($2);
		}
	|  motive
		{
		if(bug)printf("%s$%s$\n----------\n",
			"stat 4: motive=\n",
			$1);
		$$ = scpy(1, $1);
		free($1);
		}
	|  stat PLAYER 
		{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"stat 5: stat PLAYER=\n",
			$1, $2);
		$$ = scpy(3, $1, white, $2);
		free($1); free($2);
		}
	;

tstat	: stat LSEMI
		{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"tstat: stat LSEMI=\n",
			$1, $2);
		$$ = scpy(2, $1, $2);
		free($1); free($2);
		}
	;

cyclh	: CYCLIST
		{
		if (!inplayer)
			yyerror("Playery: cyclic list not in body of Player");
		$$ = scpy(2, white, $1);
		free($1);
		parencnt = 0;
		countparens = 1;
		}
	;

cycl	:  cyclh LLPAREN stat LRPAREN
		{
		if(bug)printf("%s$%s$%s$%s$%s$\n----------\n",
			"cycl: cyclh LLPAREN stat LRPAREN=\n",
			$1, $2, $3, $4);
		nl2sp($3);
		$$ = scpy(6, $1, $2, "\"", $3, "\"", $4);
		free($1); free($2); free($3); free($4);
		countparens = 0;
		}
	;

genh	: GEN 
		{
		$$ = scpy(2, white, $1);
		free($1);
		parencnt = 0;
		countparens = 1;
		}
	;

gen	:  genh LLPAREN stat LRPAREN
		{
		if(bug)printf("%s$%s$%s$%s$%s$\n----------\n",
			"cycl: genh LLPAREN stat LRPAREN=\n",
			$1, $2, $3, $4);
		nl2sp($3);
		$$ = scpy(6, $1, $2, "\"", $3, "\"", $4);
		free($1); free($2); free($3); free($4);
		countparens = 0;
		}
	;

motivh	: MOTIVE 
		{
		if (!inplayer)
			yyerror("Playery: Motive not in body of Player");
		$$ = scpy(2, white, $1);
		free($1);
		parencnt = 0;
		countparens = 1;
		}
	;

motive	:  motivh LLPAREN stat LRPAREN
		{
		if(bug)printf("%s$%s$%s$%s$%s$\n----------\n",
			"motive: motivh LLPAREN stat LRPAREN=\n",
			$1, $2, $3, $4);
		nl2sp($3);
		$$ = scpy(6, $1, $2, "\"", $3, "\"", $4);
		free($1); free($2); free($3); free($4);
		countparens = 0;
		}
	;

obj	:  PNFIELD
		{ 
		char *c;
		if (!inplayer)
			yyerror("Playery: P field not in body of Player");
		c = getpnum($1);
		$$ = scpy(4, white, "_x->P_pn[", c, "]");
		vs[playercnt].hi_ref_pn = max(vs[playercnt].hi_ref_pn, atoi(c));
		free($1);
		}
	|  PSFIELD
		{ 
		char *c;
		if (!inplayer)
			yyerror("Playery: P field not in body of Player");
		c = getpnum($1);
		$$ = scpy(4, white, "_x->P_ps[", c, "]");
		vs[playercnt].hi_ref_pn = max(vs[playercnt].hi_ref_pn, atoi(c));
		free($1);
		}
	|  PIFIELD
		{ 
		char *c;
		if (!inplayer)
			yyerror("Playery: P field not in body of Player");
		c = getpnum($1);
		$$ = scpy(4, white, "_x->P_pi[", c, "]");
		vs[playercnt].hi_ref_pn = max(vs[playercnt].hi_ref_pn, atoi(c));
		free($1);
		}
	|  PPFIELD
		{ 
		char *c;
		if (!inplayer)
			yyerror("Playery: P field not in body of Player");
		c = getpnum($1);
		$$ = scpy(4, white, "_x->P_pp[", c, "]");
		vs[playercnt].hi_ref_pn = max(vs[playercnt].hi_ref_pn, atoi(c));
		free($1);
		}
	|  P_START LLCRLY
		{ 
		if (!inplayer)
			yyerror("Playery: Start block not in body of Player");
		$$ = scpy(3, white, "if(_x->P_icode==0)", $2);
		crlycnt++;
		free($1); free($2);
		}
	|  M_START 
		{ 
		$$ = scpy(2, white, $1);
		m_start_seen++;
		free($1); 
		}
	|  M_TER
		{ 
		$$ = scpy(2, white, $1);
		t_start_seen++;
		free($1); 
		}
	|  LPERIOD
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LINT   
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LFLOAT
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LSYMBOL
		{ 
		if (lookup($1)) {
			if(bug)printf("%s$%s$", "Instance symbol: ", $1);
			$$ = scpy(5, white, "((struct ", vs[playercnt].player,
				" *)_x->P_nstnc)->", $1);
		} else {
			if(bug)printf("%s$%s$\n", "lsymbol:", $1);
			$$ = scpy(2, white, $1); 
		}
		if (bug)printf("----------\n");
		free($1);
		}
	|  LCOMMA
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LOCTAL 
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LHEX
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LQSTR
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LBRACKETS
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LOP
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LQCHR
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LCOMMENT
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LERROR
		{ 
		yyerror("Playery: error in lexer");
		exit(1);
		}
	|  LLPAREN
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LRPAREN
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LLCRLY
		{ 
		crlycnt++;
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LRCRLY
		{ 
		crlycnt--;
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LLSQB
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LRSQB
		{ 
		$$ = scpy(2, white, $1); 
		free($1);
		}
	|  LARB
		{ /* used to parse parens when in cyclic lists */
		$$ = scpy(2, white, $1); 
		free($1);
		}
	| EXTERN
		{ /* used to parse parens when in cyclic lists */
		$$ = scpy(2, white, $1); 
		free($1);
		}
	;  

%%

char *
scpy(a)
	int a;
{
	char *tmp;
	int i, *p, len = 0; 
	char **x;

	p = &a;		/* p points to head of args */
	i = *p++;	/* p points to first string */
	x = (char **) p;/* x points to first string */
	while (i--) {
		len += strlen(*x++);
	}
	tmp = (char *) malloc(len+1);
	i = a;
	x = (char **) p;
	*tmp = '\0';
	while (i--) {
		strcat(tmp, *x++);
	}
	return(tmp);
}

max(x, y)
	int x, y;
{
	return(x>y ? x : y);
}

char *getpnum(str)
	char *str;
{
	if (isdigit(str[1]))	/* it's a numeric p field */
		str++;
	else
		str += 2;	/* skip over second char */
	return(str);
}

nl2sp(str)
	char *str;
{
	char *c;

	for (c = str; *c != NULL; c++)
		if (*c == '\n')
			*c = ' ';
}
