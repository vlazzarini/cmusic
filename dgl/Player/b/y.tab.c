
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

typedef union  {
	int i;
	char *s;
	} YYSTYPE;
# define LSTART 257
# define LPERIOD 258
# define LINT 259
# define LFLOAT 260
# define LWHITE 261
# define LSYMBOL 262
# define LCOMMA 263
# define LOCTAL 264
# define LHEX 265
# define LQSTR 266
# define LBRACKETS 267
# define LOP 268
# define LQCHR 269
# define LARB 270
# define LSEMI 271
# define LCOMMENT 272
# define LERROR 273
# define LLPAREN 274
# define LRPAREN 275
# define LLCRLY 276
# define LRCRLY 277
# define LLSQB 278
# define LRSQB 279
# define PLAYER 280
# define EXTERN 281
# define PNFIELD 282
# define PIFIELD 283
# define PSFIELD 284
# define PPFIELD 285
# define CYCLIST 286
# define V_ST_REF 287
# define GEN 288
# define P_START 289
# define M_START 290
# define M_TER 291
# define INSTANCE 292
# define MOTIVE 293
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256



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
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 6,
	271, 59,
	286, 59,
	288, 59,
	-2, 6,
	};
# define YYNPROD 64
# define YYLAST 275
short yyact[]={

  18,  19,  20,  54,  21,  22,  23,  24,  25,  26,
  27,  28,  36,  58,  29,  30,  31,  32,  33,   6,
  34,  35,  10,  37,  11,  13,  12,  14,  57,  56,
  55,  15,  16,  17,   5,  39,  18,  19,  20,  40,
  21,  22,  23,  24,  25,  26,  27,  28,  36,  45,
  29,  30,  31,  32,  33,  46,  34,  35,  44,  37,
  11,  13,  12,  14,  49,  53,  50,  15,  16,  17,
  18,  19,  20,   2,  21,  22,  23,  24,  25,  26,
  27,  28,  36,  38,  29,  30,  31,  76,  33,  46,
  34,  35,  44,  37,  11,  13,  12,  14,  49,   9,
  50,  15,  16,  17,  18,  19,  20,  48,  21,  22,
  23,  24,  25,  26,  27,  28,  36,  43,  29,  30,
  31,  75,  33,  46,  34,  35,  44,  37,  11,  13,
  12,  14,  49,  47,  50,  15,  16,  17,  18,  19,
  20,   7,  21,  22,  23,  24,  25,  26,  27,  28,
  36,  42,  29,  30,  31,  74,  33,  46,  34,  35,
  44,  37,  11,  13,  12,  14,  49,  62,  50,  15,
  16,  17,  18,  19,  20,   1,  21,  22,  23,  24,
  25,  26,  27,  28,  36,   0,  29,  30,  31,  32,
  33,  46,  34,  35,  63,  37,  11,  13,  12,  14,
   0,   0,   8,  15,  16,  17,  41,  39,  65,  66,
   3,  64,  79,  67,  68,   4,  52,  73,   0,  69,
   0,  51,   0,  70,  77,   0,   0,  71,  72,  65,
  66,   0,  64,   0,  67,  68,   0,   0,  73,   0,
  69,   0,   0,   0,  70,   0,   0,   0,  71,  72,
   0,   0,   0,   0,   0,  41,   0,  78,   0,   0,
   0,   0,  41,  41,  41,   0,  59,  60,  61,   0,
   0,   0,   0,   0,  80 };
short yypact[]={

-183,-258,-232,-222,-1000, -86,-1000,-197,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-273,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-244,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-245,-246,-1000,
-1000,-1000,-222,-261,-1000, -86, -86, -86, -30,-120,
-154,-188, -51,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000, -30,
-1000 };
short yypgo[]={

   0, 175, 210, 215, 202, 167, 194, 151, 141, 133,
 117, 107,  99,  83 };
short yyr1[]={

   0,   1,   1,   1,   1,   1,   1,   1,   8,   5,
   5,   5,   5,   6,   6,   6,   6,   6,   6,   6,
   6,   6,   6,   2,   2,   2,   2,   2,   2,   3,
   9,   7,  11,  10,  13,  12,   4,   4,   4,   4,
   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
   4,   4,   4,   4 };
short yyr2[]={

   0,   0,   2,   2,   2,   3,   2,   6,   1,   0,
   1,   2,   3,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   2,   2,   2,   1,   2,   2,
   1,   4,   1,   4,   1,   4,   1,   1,   1,   1,
   2,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1 };
short yychk[]={

-1000,  -1, 256,  -2,  -3, 292, 277,  -8,  -4, -12,
 280, 282, 284, 283, 285, 289, 290, 291, 258, 259,
 260, 262, 263, 264, 265, 266, 267, 268, 269, 272,
 273, 274, 275, 276, 278, 279, 270, 281, -13, 293,
 271,  -4,  -7, -10, 280, 271, 277,  -9, -11, 286,
 288,  -3,  -2, 262, 276, 274, 274, 274, 274,  -2,
  -2,  -2,  -5,  -6, 262, 259, 260, 264, 265, 270,
 274, 278, 279, 268, 275, 275, 275, 275,  -6, 263,
  -6 };
short yydef[]={

   1,  -2,   0,   3,   4,   0,  -2,   0,  23,  27,
   8,  36,  37,  38,  39,   0,  41,  42,  43,  44,
  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,
  55,  56,  57,  58,  60,  61,  62,  63,   0,  34,
   2,  24,  25,  26,  28,  29,  59,   0,   0,  30,
  32,   5,   0,   0,  40,   0,   0,   0,   9,   0,
   0,   0,   0,  10,  13,  14,  15,  16,  17,  18,
  19,  20,  21,  22,  35,  31,  33,   7,  11,   0,
  12 };
#ifndef lint
static	char yaccpar_sccsid[] = "@(#)yaccpar 1.1 84/12/21 SMI"; /* from UCB 4.1 83/02/11 */
#endif

#
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

#ifdef YYDEBUG
	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
#endif
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

#ifdef YYDEBUG
			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

#ifdef YYDEBUG
			if( yydebug ) printf( "error recovery discards char %d\n", yychar );
#endif

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

#ifdef YYDEBUG
		if( yydebug ) printf("reduce %d\n",yyn);
#endif
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 1:
{ yyval.s = NULL; } break;
case 2:
{} break;
case 3:
{
		if(bug)printf("%s$%s$\n----------\n", 
			"start 1: start stat=\n",
			yypvt[-0].s);
		foo = divert(&divert_body, &divert_body_cnt,
			&divert_body_len, yypvt[-0].s);
		free(yypvt[-0].s);
		} break;
case 4:
{
		if(bug)printf("%s$%s$\n----------\n", 
			"start 2: start tstat=\n",
			yypvt[-0].s);
		foo = divert(&divert_body, &divert_body_cnt,
			&divert_body_len, yypvt[-0].s);
		free(yypvt[-0].s);
		} break;
case 5:
{
		if(bug)printf("%s$%s$%s$\n----------\n", 
			"start 2a: start INSTANCE tstat=\n",
			yypvt[-1].s, yypvt[-0].s);
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
			&divert_instance_decl_len, yypvt[-0].s);
		parse_decl(yypvt[-0].s);
		free(yypvt[-1].s);
		free(yypvt[-0].s);
		} break;
case 6:
{
		if(bug)printf("%s$%s$\n----------\n",
			"start 3: start LRCRLY=\n", 
			yypvt[-0].s);
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
				yypvt[-0].s); 
			inplayer = 0;
			first_inst = 0;
			clear_hash();
		} else {
			char *x = scpy(3, white, yypvt[-0].s, "\n");
			foo = divert(&divert_body, &divert_body_cnt,
				&divert_body_len, x);
			free(x);
		}
		if (crlycnt < 0)
		    yyerror("Playery: more right than left curly braces seen");
		free(yypvt[-0].s);
		} break;
case 7:
{
		if(bug)printf("%s$%s$%s$%s$%s$%s$\n----------\n",
			"start 4: start player LSYMBOL LLPAREN args LRPAREN=\n",
			yypvt[-4].s, yypvt[-3].s, yypvt[-2].s, yypvt[-1].s, yypvt[-0].s);
		foo = undivert(&divert_body, &divert_body_cnt,
			&divert_body_len);
		{
			char *x = scpy(7, yypvt[-4].s, "Player\n",
				yypvt[-3].s, yypvt[-2].s, "_x", yypvt[-0].s, "\n\tPlayer *_x;");
			foo = divert(&divert_body, &divert_body_cnt,
				&divert_body_len, x);
			free(x);
		}
		vs[playercnt].player = yypvt[-3].s;
		if (yypvt[-1].s != NULL) {			/* any args? */
			char *x, *y, *z, *index();	/* yes */
			int	flag = 1;

			/* how many args? */
			if ((x = index(yypvt[-1].s, ',')) != NULL) {
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
				vs[playercnt].begin = scpy(1, yypvt[-1].s);
				if (integer)
				    vs[playercnt].dur = scpy(1, "-1L");
				else
				    vs[playercnt].dur = scpy(1, "-1.0");
				vs[playercnt].stat = scpy(1, "P_READY");
				vs[playercnt].cnt = scpy(1, "1");
				break;
			case 2:
				vs[playercnt].begin = scpy(1, yypvt[-1].s);
				vs[playercnt].dur = scpy(1, x+1);
				vs[playercnt].stat = scpy(1, "P_READY");
				vs[playercnt].cnt = scpy(1, "1");
				break;
			case 3:
				vs[playercnt].begin = scpy(1, yypvt[-1].s);
				vs[playercnt].dur = scpy(1, x+1);
				vs[playercnt].stat = scpy(1, y+1);
				vs[playercnt].cnt = scpy(1, "1");
				break;
			case 4:
				vs[playercnt].begin = scpy(1, yypvt[-1].s);
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
		free(yypvt[-4].s); free(yypvt[-2].s); 
		if (yypvt[-1].s != NULL) 
			free(yypvt[-1].s); 
		free(yypvt[-0].s);
		} break;
case 8:
{
		yyval.s = scpy(1, white);
		free(yypvt[-0].s);
		countparens = 1;
		parencnt = 0;
		playercnt++;
		} break;
case 9:
{ 
		if(bug)printf("arg 0: <NULL>\n----------\n");
		yyval.s = NULL; 
		} break;
case 10:
{
		if(bug)printf("%s$%s$\n----------\n",
			"arg 1: arg_obj=\n",
			yypvt[-0].s);
		yyval.s = scpy(1, yypvt[-0].s);
		free(yypvt[-0].s); 
		} break;
case 11:
{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"arg3: args arg_obj=\n",
			yypvt[-1].s, yypvt[-0].s);
		yyval.s = scpy(2, yypvt[-1].s, yypvt[-0].s);
		free(yypvt[-0].s);
		} break;
case 12:
{
		if(bug)printf("%s$%s$%s$%s$\n----------\n",
			"arg4: args LCOMMA arg_obj=\n",
			yypvt[-2].s, yypvt[-1].s, yypvt[-0].s);
		yyval.s = scpy(3, yypvt[-2].s, yypvt[-1].s, yypvt[-0].s);
		free(yypvt[-1].s); free(yypvt[-0].s);
		} break;
case 13:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 14:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 15:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 16:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 17:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 18:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 19:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 20:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 21:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 22:
{ yyval.s = scpy(2, white, yypvt[-0].s); free(yypvt[-0].s); } break;
case 23:
{
		if(bug)printf("%s$%s$\n----------\n",
			"stat 0: obj=\n",
			yypvt[-0].s);
		yyval.s = scpy(1, yypvt[-0].s);
		free(yypvt[-0].s); 
		} break;
case 24:
{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"stat 1: stat obj=\n",
			yypvt[-1].s, yypvt[-0].s);
		yyval.s = scpy(2, yypvt[-1].s, yypvt[-0].s);
		free(yypvt[-1].s); free(yypvt[-0].s);
		} break;
case 25:
{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"stat2: stat cycl=\n",
			yypvt[-1].s, yypvt[-0].s);
		yyval.s = scpy(2, yypvt[-1].s, yypvt[-0].s);
		free(yypvt[-1].s); free(yypvt[-0].s);
		} break;
case 26:
{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"stat 3: stat gen=\n",
			yypvt[-1].s, yypvt[-0].s);
		yyval.s = scpy(2, yypvt[-1].s, yypvt[-0].s);
		free(yypvt[-1].s); free(yypvt[-0].s);
		} break;
case 27:
{
		if(bug)printf("%s$%s$\n----------\n",
			"stat 4: motive=\n",
			yypvt[-0].s);
		yyval.s = scpy(1, yypvt[-0].s);
		free(yypvt[-0].s);
		} break;
case 28:
{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"stat 5: stat PLAYER=\n",
			yypvt[-1].s, yypvt[-0].s);
		yyval.s = scpy(3, yypvt[-1].s, white, yypvt[-0].s);
		free(yypvt[-1].s); free(yypvt[-0].s);
		} break;
case 29:
{
		if(bug)printf("%s$%s$%s$\n----------\n",
			"tstat: stat LSEMI=\n",
			yypvt[-1].s, yypvt[-0].s);
		yyval.s = scpy(2, yypvt[-1].s, yypvt[-0].s);
		free(yypvt[-1].s); free(yypvt[-0].s);
		} break;
case 30:
{
		if (!inplayer)
			yyerror("Playery: cyclic list not in body of Player");
		yyval.s = scpy(2, white, yypvt[-0].s);
		free(yypvt[-0].s);
		parencnt = 0;
		countparens = 1;
		} break;
case 31:
{
		if(bug)printf("%s$%s$%s$%s$%s$\n----------\n",
			"cycl: cyclh LLPAREN stat LRPAREN=\n",
			yypvt[-3].s, yypvt[-2].s, yypvt[-1].s, yypvt[-0].s);
		nl2sp(yypvt[-1].s);
		yyval.s = scpy(6, yypvt[-3].s, yypvt[-2].s, "\"", yypvt[-1].s, "\"", yypvt[-0].s);
		free(yypvt[-3].s); free(yypvt[-2].s); free(yypvt[-1].s); free(yypvt[-0].s);
		countparens = 0;
		} break;
case 32:
{
		yyval.s = scpy(2, white, yypvt[-0].s);
		free(yypvt[-0].s);
		parencnt = 0;
		countparens = 1;
		} break;
case 33:
{
		if(bug)printf("%s$%s$%s$%s$%s$\n----------\n",
			"cycl: genh LLPAREN stat LRPAREN=\n",
			yypvt[-3].s, yypvt[-2].s, yypvt[-1].s, yypvt[-0].s);
		nl2sp(yypvt[-1].s);
		yyval.s = scpy(6, yypvt[-3].s, yypvt[-2].s, "\"", yypvt[-1].s, "\"", yypvt[-0].s);
		free(yypvt[-3].s); free(yypvt[-2].s); free(yypvt[-1].s); free(yypvt[-0].s);
		countparens = 0;
		} break;
case 34:
{
		if (!inplayer)
			yyerror("Playery: Motive not in body of Player");
		yyval.s = scpy(2, white, yypvt[-0].s);
		free(yypvt[-0].s);
		parencnt = 0;
		countparens = 1;
		} break;
case 35:
{
		if(bug)printf("%s$%s$%s$%s$%s$\n----------\n",
			"motive: motivh LLPAREN stat LRPAREN=\n",
			yypvt[-3].s, yypvt[-2].s, yypvt[-1].s, yypvt[-0].s);
		nl2sp(yypvt[-1].s);
		yyval.s = scpy(6, yypvt[-3].s, yypvt[-2].s, "\"", yypvt[-1].s, "\"", yypvt[-0].s);
		free(yypvt[-3].s); free(yypvt[-2].s); free(yypvt[-1].s); free(yypvt[-0].s);
		countparens = 0;
		} break;
case 36:
{ 
		char *c;
		if (!inplayer)
			yyerror("Playery: P field not in body of Player");
		c = getpnum(yypvt[-0].s);
		yyval.s = scpy(4, white, "_x->P_pn[", c, "]");
		vs[playercnt].hi_ref_pn = max(vs[playercnt].hi_ref_pn, atoi(c));
		free(yypvt[-0].s);
		} break;
case 37:
{ 
		char *c;
		if (!inplayer)
			yyerror("Playery: P field not in body of Player");
		c = getpnum(yypvt[-0].s);
		yyval.s = scpy(4, white, "_x->P_ps[", c, "]");
		vs[playercnt].hi_ref_pn = max(vs[playercnt].hi_ref_pn, atoi(c));
		free(yypvt[-0].s);
		} break;
case 38:
{ 
		char *c;
		if (!inplayer)
			yyerror("Playery: P field not in body of Player");
		c = getpnum(yypvt[-0].s);
		yyval.s = scpy(4, white, "_x->P_pi[", c, "]");
		vs[playercnt].hi_ref_pn = max(vs[playercnt].hi_ref_pn, atoi(c));
		free(yypvt[-0].s);
		} break;
case 39:
{ 
		char *c;
		if (!inplayer)
			yyerror("Playery: P field not in body of Player");
		c = getpnum(yypvt[-0].s);
		yyval.s = scpy(4, white, "_x->P_pp[", c, "]");
		vs[playercnt].hi_ref_pn = max(vs[playercnt].hi_ref_pn, atoi(c));
		free(yypvt[-0].s);
		} break;
case 40:
{ 
		if (!inplayer)
			yyerror("Playery: Start block not in body of Player");
		yyval.s = scpy(3, white, "if(_x->P_icode==0)", yypvt[-0].s);
		crlycnt++;
		free(yypvt[-1].s); free(yypvt[-0].s);
		} break;
case 41:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s);
		m_start_seen++;
		free(yypvt[-0].s); 
		} break;
case 42:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s);
		t_start_seen++;
		free(yypvt[-0].s); 
		} break;
case 43:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 44:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 45:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 46:
{ 
		if (lookup(yypvt[-0].s)) {
			if(bug)printf("%s$%s$", "Instance symbol: ", yypvt[-0].s);
			yyval.s = scpy(5, white, "((struct ", vs[playercnt].player,
				" *)_x->P_nstnc)->", yypvt[-0].s);
		} else {
			if(bug)printf("%s$%s$\n", "lsymbol:", yypvt[-0].s);
			yyval.s = scpy(2, white, yypvt[-0].s); 
		}
		if (bug)printf("----------\n");
		free(yypvt[-0].s);
		} break;
case 47:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 48:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 49:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 50:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 51:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 52:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 53:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 54:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 55:
{ 
		yyerror("Playery: error in lexer");
		exit(1);
		} break;
case 56:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 57:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 58:
{ 
		crlycnt++;
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 59:
{ 
		crlycnt--;
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 60:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 61:
{ 
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 62:
{ /* used to parse parens when in cyclic lists */
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
case 63:
{ /* used to parse parens when in cyclic lists */
		yyval.s = scpy(2, white, yypvt[-0].s); 
		free(yypvt[-0].s);
		} break;
		}
		goto yystack;  /* stack new state and value */

	}
