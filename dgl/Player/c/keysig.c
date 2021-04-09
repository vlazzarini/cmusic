/* %M%	%I%	(CARL)	%G%	%U% */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <carl/Player.h>

set_keysig(keyspec, k)
	char *keyspec;
	struct sig_key *k;
{
  extern char *index();
	register char *c, save;
	register int ks, dir, i;

	if (k == NULL)
		return;

	if ((k->key_sig = malloc(strlen(keyspec)+1)) == NULL)
		malerr("set_keysig", 1);
	(void) strcpy(k->key_sig, keyspec);

	if ((c = index(keyspec, 's')) == NULL)
		if ((c = index(keyspec, 'f')) == NULL) {
			fprintf(stderr, "invalid key signature: ``%s''\n",
				keyspec);
			Pexit(1);
		}
	switch (*c) {
	case 's': dir = 1;	break;
	case 'f': dir = -1;	break;
	default: 
		fprintf(stderr, "invalid key signature: ``%s''\n", keyspec);
		Pexit(1);
	}
	save = *c;
	*c =  '\0';
	ks = atoi(keyspec);
	*c = save;
	for (i = 0; i < 12; i++)	/* clear old key signature */
		k->key_pc[i] = 0;
	if (dir == 1) {			/* set new one */
		for (i = 5; ks > 0; i += 7, ks--) {
			if (i >= 12) 
				i -= 12;
			k->key_pc[i] = dir;
		}
	} else {
		for (i = 11; ks > 0; i -= 7, ks--) {
			if (i < 0) 
				i += 12;
			k->key_pc[i] = dir;
		}
	}
	Player_stat |= P_KEYSIG;
}

set_ac(pc, ac, k)
	int pc;
	int ac;
	struct sig_key *k;
{
	int dir;

	if (k == NULL)
		return;
	if (ac == 0)
		dir = 32;
	else
		dir = ac;
	if (0 <= pc && pc < 12)
		k->key_acc[pc] = dir;
	else {
		fprintf(stderr, "set_ac: invalid pitch class: %d\n", pc);
		Pexit(1);
	}
}

clr_ac(k)
	struct sig_key *k;
{
	register int i;

	if (k == NULL)
		return;

	for (i = 0; i < 12; i++)
		k->key_acc[i] = 0;
}

int
get_ac(pc, k)
	int pc;
	struct sig_key *k;
{
	if (k == NULL)
		return(0);
	if (0 <= pc && pc < 12) {
		if (k->key_acc[pc] != 0)
			return(k->key_acc[pc] == 32 ? 0 : k->key_acc[pc]);
		else
			return(k->key_pc[pc]);
	} else {
		fprintf(stderr, "invalid pitch class: %d\n", pc);
		Pexit(1);
	}
}
