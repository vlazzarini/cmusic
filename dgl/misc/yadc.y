%{
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

%}

%start  list

%token  NUMBER  LETTER ERROR

%left  '|' 
%left  '^'
%left  '&'
%left  '<'  '>'
%left  '+'  '-'
%left  '*'  '/'  '%'
%right '@'
%left  '~'
%left  UMINUS      /*  supplies  precedence  for  unary  minus  */
%left  EXCL	   /*  supplies  precedence  for  factorial     */

%%      /*  beginning  of  rules  section  */

list :    /*  empty  */
     |    list  stat  '\n'
     |    list  error  '\n'
               {    yyerrok;  }
     ;

stat :    expr
               {    preprint(pdflt[1]);
		    printf( pdflt, $1 );  }
     |    LETTER  '='  expr
               {    regs[forcelower($1)-'a']  =  $3;  }
     |    prefix
	       {    setprint($1); }
     |    prefix expr
               {    store = setprint($1);
		    preprint(pdflt[1]);
		    printf( pdflt, $2 );  
		    if (!sticky) setprint(store);
		    }
     ;

prefix :  '%' LETTER
	       {    $$ =   $2;  sticky = 0; }
     |    '$' LETTER
	       {    $$ =   $2;  sticky++; }
     ;

expr :    '('  expr  ')'
               {    $$  =  $2;  }
     |    expr  '+'  expr
               {    $$  =  $1  +  $3;  }
     |    expr  '-'  expr
               {    $$  =  $1  -  $3;  }
     |    expr  '*'  expr
               {    $$  =  $1  *  $3;  }
     |    expr  '@'  expr
               {    t1 = $1; t2 = $3;
		    t3 =  pow(t1,t2);  
		    $$ = t3; }
     |    expr  '/'  expr
               {    $$  =  $1  /  $3;  }
     |    expr  '%'  expr
               {    $$  =  $1  %  $3;  }
     |    expr  '&'  expr
               {    $$  =  $1  &  $3;  }
     |    expr '<' '<'  expr
               {    $$  =  $1  <<  $4;  }
     |    expr  '>' '>' expr
               {    $$  =  $1  >>  $4;  }
     |    expr  '|'  expr
               {    $$  =  $1  |  $3;  }
     |    expr  '^'  expr
               {    $$  =  $1  ^  $3;  }
     |    LETTER
               {    $$  =  regs[forcelower($1)-'a'];  }
     |    '-'  expr        %prec  UMINUS
               {    $$  =  -  $2;  }
     |    expr  '!'	   %prec EXCL
               {    $$  =  factorial($1);  }
     |    '~'  expr
               {    $$  =  ~  $2;  }
     |    NUMBER
		{	$$ = $1; }
     ;

%%      /*  start  of  programs  */

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
