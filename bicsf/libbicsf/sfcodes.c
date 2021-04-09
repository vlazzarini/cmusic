#include <stdio.h>
#include <sys/file.h>
#include <sfheader.h>

	/*
	 *	getsfcode() searches a header for a specified
	 *	code.  If the code is found, getsfcode returns
	 *	a pointer to the beginning of the SFCODE structure not
	 *	to the information structure itself. 
	 *	The srate, number of channels, magicnumber, 
	 *	number of bytes per channel are NOT coded via these routines. 
	 */

SFCODE *getsfcode(hd,code)
	SFHEADER *hd;
{
	register char *sfc;
	register SFCODE *sp;
	char *hdend = (char *) hd + sizeof(SFHEADER);

	sfc = &sfcodes(hd); 
	while(sfc < hdend) {
		sp = (SFCODE *) sfc;
		if(sp->code == code)
			return(sp);
			/* return(sfc); */
		if(sp->code == SF_END)
			break;
		if(sp->bsize < sizeof(SFCODE)) 
		/* Catch possible wrap around on stack from bad header */
			break;
		else
		/* keep looking */
			sfc += sp->bsize;
	}
	return(NULL);
}

SFCODE endcode = {
	SF_END,
	sizeof(SFCODE)
} ;

	/*
	 *	putsfcode() inserts new data into a soundfile header.
	 *	If such data (data having the same sfcode) already exists,
	 *	putsfcode replaces the old data with the new. 
	 *	(size of the old and of the new data can be different). 
	 *	If the data is new to the header, it is added
	 *	at the end of the previous sfcodes.
	 */

putsfcode(hd,data,new)
	SFHEADER *hd;
	char *data;
	SFCODE *new;
{
	register char *sfc,*next;
	register SFCODE *sp,*old = NULL, *end = NULL;
	char *save, *hdend = (char *) hd + sizeof(SFHEADER);

	sfc = &sfcodes(hd); 
	while(sfc <= hdend) {
		sp = (SFCODE *) sfc;
		if(sp->code == new->code){  
		/* previous data to be replaced */
			if(old)
				return(-1);
			old = sp;
			next = (char *)sp + sp->bsize;
		}
		if(sp->code == SF_END) {  
		/* new data...put at end */
			sfc += sizeof(SFCODE);
			end = sp;
			break;
		}
		if(sp->bsize < sizeof(SFCODE) || sfc + sp->bsize > hdend) {
		/* Catch possible wrap around on stack from bad header */
			sp->code = SF_END; /* Force an end */
			sp->bsize = sizeof(SFCODE);
			sfc += sizeof(SFCODE);
			end = sp;
			break;
		}
		else				
		/* keep looking */
			sfc += sp->bsize;
	}
	if(old){    
	/* replace old data */
		if(old->bsize==new->bsize){
		/* if sizes match perfectly */
			bcopy((char *) new, (char *) old, sizeof(SFCODE));
			bcopy(data, (char *) old + sizeof(SFCODE), 
				new->bsize - sizeof(SFCODE));
			return(0);
		}
		/* 
		 * else store the rest of the header data (sfc - next), 
		 * copy in new data, then copy back the old data
		 */
		if(((char *) old + new->bsize + (sfc - next)) > hdend)
			return(-1);
		save = (char *) malloc(sfc-next);   
		bcopy(next,save,sfc-next);
		bcopy((char *) new, (char *) old, sizeof(SFCODE));
		bcopy(data, (char *) old + sizeof(SFCODE), 
			new->bsize - sizeof(SFCODE));
		bcopy(save, (char *)old + new->bsize, sfc-next);
		return(0);
	}
	if(end){    
	/* data is new, insert at previous end location */
		if(sfc + new->bsize > hdend)
			return(-1);
		bcopy((char *) new, (char *) end, sizeof(SFCODE));
		bcopy(data, (char *) end + sizeof(SFCODE), 
			new->bsize - sizeof(SFCODE));
		bcopy(&endcode, (char *) end + new->bsize, sizeof(SFCODE));
		return(0);
	}
	return(-1);
}
