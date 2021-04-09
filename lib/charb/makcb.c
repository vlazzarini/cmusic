# include <stdio.h>
# include "charb.h"

charb *
makcb(str)
char *str;
{
	register charb *cb;

	if ((cb = cballoc(strlen(str)+1)) == NULL)
		return(NULL);

	strcpy(cb->c_buf, str);

	return(cb);
}
