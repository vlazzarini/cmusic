/* trans.h	2.1	(CARL)	10/26/85	23:49:59 */

/* character categories:
	a	#, $, @, `
	#	NL (newline)
	0	undefined (includes all ^ chrs not considered white space)
	l	[g-m,o-q,s,u-w,y-z,]
	h	[a,c,d]
	b	b
	e	e
	f	f
	x	x
	j	[n,r,t]
	.	.
	o	[1-7]	(all octal numbers)
	d	[8-9]	(remaining decimals)
	z	0
	w	sp, ht, vt, lf, cr
	q	"
	s	'
	+	+
	-	-
	r	%<=!&^|?:	(compound operators are only treated singly)
	/	/
	*	*
	>	>
	c	,
	y	\	
	;	;
	(){}[]	appear as themselves
__________________________________________

class		characters
------------------------------------------
letters		abcdefghijklmnopqrstuvwxyz
hex chr digits	abcdef
escapes		 b   f       n   r t
hex prefix	                       x
floating pt	    e

char. class	characters
__________________________________________
l		      ghijklm opq s uvw yz
h		a cd
j		             n   r t
b		 b
e		    e
f		     f
x		                       x

global classes		lexemes
__________________________________________
all letters		lhjbefx
all hex chr digits	hbef
all escapes		bfjy
hex prefix		x
floating pt		e
all white space		w#
all operators		r>+*-/	(operators are only treated singly)
all arbitrary		a
all characters		lhjbefxodzw#r>+*-(){}[]qsc;./a*y
			(letters, digits, white space, operators, 
			brockets, brackets, quotes, comma, 
			semicolon, period, arbitrary)
all brackets		(){}[]
all digits		odz
*/

struct trans
    {
    char *transchr;
    int nxtstate;
    }
    t0[] =	/* z = normal exit */
	{{NULL,0},{NULL,0}},
    t1[] =	/* ! = starting point */
	{{".",2},{"od",3},{"s",5},{"w#",9},{"lhjbefx",10},{"c",11},{"z",12},
	{"q",16},{"r>+*",19},{"-",31},{"ay",32},{";",33},{"/",34},
	{"(",38},{")",39},{"{",40},{"}",41},{"[",42},{"]",43},
	{NULL,0},{NULL,0}},
    t2[] =	/* . = period */
	{{"odz",4},{NULL,0},{NULL,0}},
    t3[] =	/* i = integer */
	{{"odz",3},{".",4},{"e",6},{NULL,0},{NULL,0}},
    t4[] =	/* f = floating point number without fraction or exp  */
	{{"odz",4},{"e",6},{NULL,0},{NULL,0}},
    t5[] =	/* 0 */
	{{"lhjbefxodzwr>+*-(){}[]qc;./a*",20},{"y",23},{NULL,0},{NULL,0}},
    t6[] =	/* 0 */
	{{"+-",7},{"odz",8},{NULL,0},{NULL,0}},
    t7[] =	/* 0 */
	{{"odz",8},{NULL,0},{NULL,0}},
    t8[] =	/* f = floating point number with/without fraction and/or exp */
	{{"odz",8},{NULL,0},{NULL,0}},
    t9[] =	/* w = white space */
	{{"w#",9},{NULL,0},{NULL,0}},
    t10[] =	/* n = symbolic name */
	{{"lhjbefxodz",10},{NULL,0},{NULL,0}},
    t11[] =	/* c = comma */
	{{NULL,0},{NULL,0}},
    t12[] =	/* 0 */
	{{"oz",13},{"x",14},{".",4},{NULL,0},{NULL,0}},
    t13[] =	/* o = octal number */
	{{"oz",13},{NULL,0},{NULL,0}},
    t14[] =	/* 0 */
	{{"odzhbef",15},{NULL,0},{NULL,0}},
    t15[] =	/* x = hexidecimal number */
	{{"odzhbef",15},{NULL,0},{NULL,0}},
    t16[] =	/* 0 */
	{{"q",17},{"y",27},{"lhjbefxodzwr>+*-/(){}[]sc;.a",16},
	{NULL,0},{NULL,0}},
    t17[] =	/* s = double-quoted string */
	{{NULL,0},{NULL,0}},
    t18[] =	/* unused */
	{{NULL,0},{NULL,0}},
    t19[] =	/* r = operatoR */
	{{NULL,0},{NULL,0}},
    t20[] =	/* 0 */
	{{"s",22},{NULL,0},{NULL,0}},
    t21[] =	/* ! = error */
	{{NULL,-1},{NULL,-1}},
    t22[] = 	/* q = quoted char */ 
	{{NULL,0},{NULL,0}},
    t23[] =	/* 0 */ 
	{{"bfjys",20},{"oz",24},{NULL,0},{NULL,0}},
    t24[] =	/* 0 */ 
	{{"oz",25},{"s",22},{NULL,0},{NULL,0}},
    t25[] =	/* 0 */ 
	{{"oz",26},{"s",22},{NULL,0},{NULL,0}},
    t26[] =	/* 0 */ 
	{{"s",22},{NULL,0},{NULL,0}},
    t27[] =	/* 0 */ 
	{{"bfjqy",16},{"oz",28},{NULL,0},{NULL,0}},
    t28[] =	/* 0 */ 
	{{"q",17},{"oz",29},{"lhjbefxodzwr>+-(){}[]sc;./a*y",16},
	{NULL,0},{NULL,0}},
    t29[] =	/* 0 */ 
	{{"q",17},{"oz",30},{"lhjbefxodzwr>+-(){}[]sc;./a*y",16},
	{NULL,0},{NULL,0}},
    t30[] =	/* 0 */ 
	{{"q",17},{"lhjbefxodzwr>+-(){}[]sc;./a*y",16},{NULL,0},{NULL,0}},
    t31[] = 	/* r = minus operator, may be part of -> operator */
	{{">",19},{NULL,0},{NULL,0}},
    t32[] =	/* a = arbitrary */
	{{NULL,0},{NULL,0}},
    t33[] = 	/* ; = semicolon */
	{{NULL,0},{NULL,0}},
    t34[] = 	/* r = slash */
	{{"*",35},{NULL,0},{NULL,0}},
    t35[] = 	/* 0 */
	{{"*",36},{"lhjbefxodzw#r>+-(){}[]sc;.aq/y",35},{NULL,0},{NULL,0}},
    t36[] = 	/* 0 */
	{{"/",37},{"lhjbefxodzw#r>+-(){}[]sc;.aq*y",35},{NULL,0},{NULL,0}},
    t37[] =	/* C = comment */
	{{NULL,0},{NULL,0}},
    t38[] =	/* ( */
	{{NULL,0},{NULL,0}},
    t39[] =	/* ) */
	{{NULL,0},{NULL,0}},
    t40[] =	/* { */
	{{NULL,0},{NULL,0}},
    t41[] =	/* } */
	{{NULL,0},{NULL,0}},
    t42[] =	/* [ */
	{{NULL,0},{NULL,0}},
    t43[] =	/* ] */
	{{NULL,0},{NULL,0}}
;
