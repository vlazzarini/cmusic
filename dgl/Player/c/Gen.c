/* Gen.c	2.1	(CARL)	1/30/86	16:33:15 */

#include <carl/Player.h>
#include <ctype.h>
#include <carl/defaults.h>

Function *Gen(cmd)
	register char *cmd;
/*
 * Get Function from pipe. 
 * .LP
 * Takes
 * complete command string to pass to popen(2).
 * Typically used to synthesize functions with gen(1carl) commands.
 * Returns
 * 'NULL' on errors otherwise, the desired Function.
 * Name element of Function structure will 
 * contain the command that generated the Function.
 * Default type is 'H_MONO_IN_X'.
 */
{
	extern char		*strcpy();
	extern FILE 		*popen(); 
	extern Function		*getgenfunc();
	register FILE 		*pp;
	register Function 	*x; 

	if ((pp = popen(cmd, "r")) == NULL) {
		fprintf(stderr, "gen: popen failed on command:\n``%s''\n", cmd);
		return(NULL);
	}
	/* read function, coerce to be mono in x if no header */
	x = read_func_fid(pp, H_MONO_IN_X);
	if ((x->fname = (char *) malloc((unsigned) strlen(cmd)+1)) == NULL)
		malerr("Gen", 1);
	(void) strcpy(x->fname, cmd);
	return(x);
}
