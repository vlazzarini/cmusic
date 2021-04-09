/* lexit.c	2.1	(CARL)	10/26/85	23:49:35 */

#include <stdio.h>
#include "yylex.h"

YYSTYPE yylval;

main()
{
	int token, yylex();
	while((token = yylex()) != EOF)
		printf("\t%d %s\n",token, yylval.s);
}
