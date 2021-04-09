
# line 2 "yadc.y"
#  include  <stdio.h>
#  include  <ctype.h>
# include <math.h>

#define forcelower(x) ((isupper(x))?tolower(x):x)

long  regs[26];
int  base;
char pdflt[] = "%d\n";
char store;
int sticky;
double t1, t2, t3;

# define NUMBER 257
# define LETTER 258
# define ERROR 259
# define UMINUS 260
# define EXCL 261
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 99 "yadc.y"
      /*  start  of  programs  */

#define		CRANGE(LB,Q,UB) ('LB' <= Q && Q <= 'UB')

yylex()
{
	int             c, base;

	while ((c = getchar()) == ' ') {	/* skip  blanks  */
	}

	/* c  is  now  nonblank  */

	if (isalpha(c)) {
		yylval = c;
		return (LETTER);
	}
	if (isdigit(c)) {
		yylval = 0;
		if (c == '0') {
			char            d;
			base = 8;
			d = getchar();
			if (d == 'x')
				base = 16;
			else
				ungetc(d, stdin);
		} else
			base = 10;
		if (base == 10)
			while (isdigit(c)) {
				yylval *= base;
				yylval += (c - '0');
				c = getchar();
				if (!isdigit(c))
					ungetc(c, stdin);
			}
		if (base == 8)
			while (CRANGE(0, c, 7)) {
				yylval *= base;
				yylval += (c - '0');
				c = getchar();
				if (!CRANGE(0, c, 7))
					ungetc(c, stdin);
			}
		if (base == 16)
			while (isdigit(c) || CRANGE(a, c, f)) {
				if (CRANGE(a, c, f)) {
					yylval *= base;
					yylval += (c - 'a' + 10);
				} else {
					yylval *= base;
					yylval += (c - '0');
				}
				c = getchar();
				if (!(isdigit(c) || CRANGE(a, c, f)))
					ungetc(c, stdin);
			}
		return (NUMBER);
	}
	if (c == '\'') {
		c = getchar();
		if (c == '\\') {
			c = getchar();
			switch (c) {
			case 0142:
				c = '\b';
				break;
			case 0164:
				c = '\t';
				break;
			case 0156:
				c = '\n';
				break;
			case 0146:
				c = '\f';
				break;
			case 0162:
				c = '\r';
				break;
			case 0134:
				c = '\\';
				break;
			case 047:
				c = '\'';
				break;
			}
			if (CRANGE(0, c, 7)) {
				int             tmp, i;
				base = 8;
				tmp = 0;
				for (i = 0; i < 3 && (CRANGE(0, c, 7)); i++) {
					tmp *= base;
					tmp += (c - '0');
					c = getchar();
					if (!CRANGE(0, c, 7)) {
						ungetc(c, stdin);
						break;
					}
				}
				if (c != '\'')
					return (ERROR);
				c = tmp;
			}
		}
		yylval = c;
		c = getchar();	/* get past remaining quote */
		if (c != '\'')
			return (ERROR);
		return (NUMBER);
	}
	return (c);
}

main()
{
	iniprint();
	return (yyparse());
}

yyerror(s)
	char           *s;
{
	fprintf(stderr, "%s\n", s);
}

char           *x;

char
setprint(c)
	char            c;
{
	char            rtn;
	rtn = *x;
	*x = c;
	return (rtn);
}

iniprint()
{
	char           *index();
	x = index(pdflt, 'd');
}

preprint(c)
	char            c;
{
	switch (c) {
	case 'o':
	case 'O':
		printf("0");
		break;
	case 'x':
	case 'X':
		printf("0x");
		break;
	default:
		break;
	}
}

factorial(i)
{
	return i > 0 ? i * factorial(i-1) : 1;
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 27
# define YYLAST 225
short yyact[]={

  12,  11,  34,  33,   7,  26,   7,  43,  26,   8,
  27,   8,  20,  21,  42,  26,  47,  17,  15,  26,
  16,  14,  19,  20,  21,  13,   6,   2,  17,  15,
   1,  16,   0,  19,   0,  22,  18,  23,   0,  18,
  26,   0,   0,   0,  20,   0,  22,  26,  23,  17,
  18,  20,  21,   0,  19,   0,  17,  15,  26,  16,
   0,  19,  20,  21,   0,   0,   0,  17,  15,  25,
  16,  18,  19,   0,  22,   0,  23,   0,  18,   0,
  25,   0,   0,   0,  26,  22,   0,  23,  20,  18,
   9,  26,   9,  17,  15,  20,  16,   0,  19,  24,
  17,  15,   0,  16,   0,  19,   0,   0,  25,   4,
  24,  22,   0,  23,   0,  18,  28,  30,  31,  32,
   0,   0,  18,   0,   0,  35,  36,  37,  38,  39,
  40,  41,   0,   0,  44,  45,   0,  46,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,  48,  49,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   3,  10,   5,  10,  29 };
short yypact[]={

-1000, -36,  15,  11, -14, -51, -34, -34, -34, -34,
-1000,-255,-256,-1000,-1000, -34, -34, -34, -34, -34,
 -34, -34, -46, -55, -34, -34,-1000, -34, -14,-1000,
 -25, -18, -18,-1000,-1000,   7,   7, -28, -28, -28,
 -28,  51, -34, -34,  14,  25, -14,-1000,  58,  58 };
short yypgo[]={

   0,  30,  27, 109,  26 };
short yyr1[]={

   0,   1,   1,   1,   2,   2,   2,   2,   4,   4,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3 };
short yyr2[]={

   0,   0,   3,   3,   1,   3,   1,   2,   2,   2,
   3,   3,   3,   3,   3,   3,   3,   3,   4,   4,
   3,   3,   1,   2,   2,   2,   1 };
short yychk[]={

-1000,  -1,  -2, 256,  -3, 258,  -4,  40,  45, 126,
 257,  37,  36,  10,  10,  43,  45,  42,  64,  47,
  37,  38,  60,  62, 124,  94,  33,  61,  -3, 258,
  -3,  -3,  -3, 258, 258,  -3,  -3,  -3,  -3,  -3,
  -3,  -3,  60,  62,  -3,  -3,  -3,  41,  -3,  -3 };
short yydef[]={

   1,  -2,   0,   0,   4,  22,   6,   0,   0,   0,
  26,   0,   0,   2,   3,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  24,   0,   7,  22,
   0,  23,  25,   8,   9,  11,  12,  13,  14,  15,
  16,  17,   0,   0,  20,  21,   5,  10,  18,  19 };
#ifndef lint
static char yaccpar_sccsid[] = "@(#)yaccpar	4.1	(Berkeley)	2/11/83";
#endif not lint

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
			
case 3:
# line 37 "yadc.y"
{    yyerrok;  } break;
case 4:
# line 41 "yadc.y"
{    preprint(pdflt[1]);
		    printf( pdflt, yypvt[-0] );  } break;
case 5:
# line 44 "yadc.y"
{    regs[forcelower(yypvt[-2])-'a']  =  yypvt[-0];  } break;
case 6:
# line 46 "yadc.y"
{    setprint(yypvt[-0]); } break;
case 7:
# line 48 "yadc.y"
{    store = setprint(yypvt[-1]);
		    preprint(pdflt[1]);
		    printf( pdflt, yypvt[-0] );  
		    if (!sticky) setprint(store);
		    } break;
case 8:
# line 56 "yadc.y"
{    yyval =   yypvt[-0];  sticky = 0; } break;
case 9:
# line 58 "yadc.y"
{    yyval =   yypvt[-0];  sticky++; } break;
case 10:
# line 62 "yadc.y"
{    yyval  =  yypvt[-1];  } break;
case 11:
# line 64 "yadc.y"
{    yyval  =  yypvt[-2]  +  yypvt[-0];  } break;
case 12:
# line 66 "yadc.y"
{    yyval  =  yypvt[-2]  -  yypvt[-0];  } break;
case 13:
# line 68 "yadc.y"
{    yyval  =  yypvt[-2]  *  yypvt[-0];  } break;
case 14:
# line 70 "yadc.y"
{    t1 = yypvt[-2]; t2 = yypvt[-0];
		    t3 =  pow(t1,t2);  
		    yyval = t3; } break;
case 15:
# line 74 "yadc.y"
{    yyval  =  yypvt[-2]  /  yypvt[-0];  } break;
case 16:
# line 76 "yadc.y"
{    yyval  =  yypvt[-2]  %  yypvt[-0];  } break;
case 17:
# line 78 "yadc.y"
{    yyval  =  yypvt[-2]  &  yypvt[-0];  } break;
case 18:
# line 80 "yadc.y"
{    yyval  =  yypvt[-3]  <<  yypvt[-0];  } break;
case 19:
# line 82 "yadc.y"
{    yyval  =  yypvt[-3]  >>  yypvt[-0];  } break;
case 20:
# line 84 "yadc.y"
{    yyval  =  yypvt[-2]  |  yypvt[-0];  } break;
case 21:
# line 86 "yadc.y"
{    yyval  =  yypvt[-2]  ^  yypvt[-0];  } break;
case 22:
# line 88 "yadc.y"
{    yyval  =  regs[forcelower(yypvt[-0])-'a'];  } break;
case 23:
# line 90 "yadc.y"
{    yyval  =  -  yypvt[-0];  } break;
case 24:
# line 92 "yadc.y"
{    yyval  =  factorial(yypvt[-1]);  } break;
case 25:
# line 94 "yadc.y"
{    yyval  =  ~  yypvt[-0];  } break;
case 26:
# line 96 "yadc.y"
{	yyval = yypvt[-0]; } break;
		}
		goto yystack;  /* stack new state and value */

	}
