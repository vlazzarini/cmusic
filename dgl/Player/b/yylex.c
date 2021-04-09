/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include "y.tab.h"
#include "ymain.h"
#include <ctype.h>

extern int bug;
extern int parencnt, countparens;
extern int notecalls;
char white[512];

yylex()
{
	extern YYSTYPE yylval;
	static mycrlycnt;
	int lexeme, type;
	extern char *tokenstr;
	char c, *tmp;

	if (bug)
		printf("entering yylex()\n");
	white[0] = '\0';
	/* send stuff cpp produces through untouched */
start:	lexeme = clexer();
	if (countparens) { /* are we in a CYCLIST expression? */
		int tmp = lexeme;
		if (lexeme == LRPAREN) {
			parencnt--;
			if (parencnt > 0) lexeme = LARB;
		}
		if (tmp == LLPAREN) parencnt++; 
	}
	if (lexeme == LRCRLY)
		mycrlycnt--;
	if (lexeme == LLCRLY)
		mycrlycnt++;
	if (lexeme == LSYMBOL) {
		if(type = ispfield(yylval.s))
			lexeme = type;
		else
		if (mycrlycnt == 0 && !strcmp(yylval.s, "Player"))
			lexeme = PLAYER;
		else
		if (!strcmp(yylval.s, "extern"))
			lexeme = EXTERN;
		else
		if (!strcmp(yylval.s, "P_start"))
			lexeme = P_START;
		else
		if (!strcmp(yylval.s, "M_start"))
			lexeme = M_START;
		else
		if (!strcmp(yylval.s, "M_ter"))
			lexeme = M_TER;
		else
		if (!strcmp(yylval.s, "Instance")) 
			lexeme = INSTANCE;
	}
	if (lexeme == LARB && yylval.s[0] == '#') {
		while((c = getchar()) != '\n') 	
			*tokenstr++ = c;
		ungetc(c, stdin);
		*tokenstr++ = '\0';
/*		printf("%s", yylval.s);*/
		goto start;
	}
	if (lexeme == LWHITE) {
		strcpy(white, yylval.s);
		goto start;
	}
	tmp = (char *)malloc(strlen(yylval.s)+1);
	strcpy(tmp, yylval.s);
	yylval.s = tmp;
	if (bug) printf("LEX=%s,%d:", yylval.s, lexeme);
	if (bug)
		printf("leaving yylex()\n");
	return(lexeme);
}

ispfield(field)
	char *field;
{
	int type = PNFIELD;

	if (field == NULL) return(0);
	if (!(*field == 'p' || *field == 'P')) return(0);
	if (*++field == '\0') return(0);
	if (!isdigit(*field)) {
		if (*field == 'i') type = PIFIELD;
		if (*field == 's') type = PSFIELD;
		if (*field == 'p') type = PPFIELD;
		field++;
	}
	if (*field == '\0') return(0);
	while (*field != '\0')
		if (!isdigit(*field++))
			return(0);
	return(type);
}
