

#include <soundfile.h>
#include <stdio.h>
#include <pwd.h>

char *getsfname(shortname)
	char *shortname;
{
	char *envptr = NULL;
	static char workspace[1024];
/* SDY
	char *sprintf(),*getenv(),*tilde();
*/
	char *getenv(),*tilde();

	if(!shortname)
		return(NULL);

	/* if(*shortname == '/' || !strncmp(shortname,"./",2)) */

	if(*shortname == '/') 
		return(shortname);

	if(*shortname == '@') 
		return(tilde(workspace,shortname));

	envptr = getenv(SFDIR);

	if(!envptr)
		return(shortname);

	sprintf(workspace,"%s/%s",envptr,shortname);
	return(workspace);
}
