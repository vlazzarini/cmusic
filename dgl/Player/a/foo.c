/* Player.c	2.5	(CARL)	10/28/88	22:12:56 */

#include <stdio.h>
#include <carl/Player.h>
#include <strings.h>
#define CPP "/lib/cpp"
#define CC "cc -c"
#define LD "ld -X /lib/crt0.o "
#define NM "nm -n"
/* make this get dynamic files */
#define TMP "/tmp/PXXXXXX"
#define FHEADER "<carl/Player.h>"
#define IHEADER "<carl/iPlayer.h>"
#define LIBS " -lPlayer -lcarl -lm -lc"
#define iLIBS " -liPlayer -lcarl -lc"
#define SUN_gLIBS " -lPlayer -lcarl -lm -lc -lg"
#define SUN_giLIBS " -liPlayer -lcarl -lm -lc -lg"
#define QP "cueue_players"
#define M_START "M_start"
#define M_TER "M_ter"
#define MAIN "main"
#define MSTART 	1
#define MTER 	2
#define HASMAIN	4

#define MONLY 1
#define CONLY 2

#define DOTPFILE 'P'
#define DOTcFILE 'c'
#define DOToFILE 'o'
extern char *suf(), *chsuf(), filtyp(), *mkinit();
extern char *malloc();
int m_flag;
int p_flag = 1;
int r_flag;
int globs;
char *globstr = "-g ";
char *defstrs[32];
int	defcnt;
char **symary;
long symcnt;
int integer;
int optimize;
int loader = 1;

main(argc, argv)
	int argc;
	char **argv;
{
	extern char *mktemp();
	float input, output;
	int cnt, cmpflg = 0;
	char ch, *Ifile, *initc, *inito;

	if ((Ifile = (char *) malloc(strlen(TMP)+3)) == NULL) {
		perror("malloc");
		exit(1);
	}
	sprintf(Ifile, "%s%s", mktemp(TMP), ".c");

	while ((ch = crack(argc, argv, "iPLOchprD|g|I|", 0)) != NULL) {
		switch (ch) {
			case 'i':
				integer = 1;
				break;
			case 'P':
				cmpflg = MONLY;
				break;
			case 'L':
				loader = 0;
				break;
			case 'O':
				optimize = 1;
				break;
			case 'c':
				cmpflg = CONLY;
				break;
			case 'I':
				Ifile = arg_option;
				break;
			case 'p':
				p_flag = 0;
				break;
			case 'r':
				r_flag++;
				break;
			case 'g':
				globs++;
				r_flag++;
				globstr[2] = arg_option[0];
				break;
			case 'D':
				defstrs[defcnt] = (char *) malloc(strlen(arg_option)+3);
				strcpy(defstrs[defcnt++], arg_option-2);
				break;
			case 'h':	
				usage(0);
				break;
			default:
				usage(1);
				break;
		}
	}

	for (cnt = arg_index; cnt < argc; cnt++) {
		switch(filtyp(argv[cnt])) {
			case DOTPFILE:	/* x.P ==> x.c */
				if (Playery(argv[cnt])) 
					exit(1);
				if (cmpflg == MONLY) 
					exit(0);
				/* no break */
			case DOTcFILE: 	/* x.c ==> x.o */
				if (cc(chsuf(argv[cnt], DOTcFILE))) 
					exit(1);
				if (cmpflg == CONLY) 
					exit(0);
				/* no break */
			case DOToFILE: 	/* collect symbols from x.o */
				if (nm(chsuf(argv[cnt], DOToFILE))) exit(1);
				break;
			default:	/* who knows? */
				break;
		}
	}
	if (symcnt == 0) {
		fprintf(stderr, "Player: no Players!\n");
		exit(1);
	}
	/* make init_players() subroutine to call players */
	initc = mkinit(Ifile);
	/* glue the whole thing together */
	if (loader)
		ld(inito = chsuf(initc, DOToFILE), &argv[arg_index]);
	if (!r_flag) {
		if (p_flag) fprintf(stderr, "rm %s\n", initc);
		unlink(initc);
		if (p_flag) fprintf(stderr, "rm %s\n", inito);
		unlink(inito);
	}
	exit(0);
}

usage(exitcode)
	int exitcode;
{
fprintf(stderr, "usage: Player [flags] {file.P,file.c,file.o}... -l...\n");
fprintf(stderr, "%s%s%s%s%s%s%s%s%s",
"flags:\n",
"-P\trun Player preprocessor only\n",
"-c\trun preprocessor and C compiler only\n",
"-L\trun preprocessor, C compiler and nm, do not run ld\n",
"-oX\tname the output file X (default: a.out)\n",
"-g\tload symbol table\n",
"-p\tdo not print actions on stderr\n",
"-r\tdo not remove temporary Player link files\n",
"-h\tprint this message\n"
);
exit(exitcode);
}


char *chsuf(file, sub)
	char *file, sub;
{
	extern char *rindex();
	char *c, *x; 
	int len = strlen(file);

	if (len <= 0)
		return(NULL);
	if ((x = rindex(file, '.')) == NULL)
		return(NULL);
	if (*(x+1) == '/')
		return(file);
	if ((x = malloc(len+1)) == NULL) {
		perror("malloc");
		exit(1);
	}
	(void) strcpy(x, file);
	if ((c = rindex(x, '.')) != NULL) {
		if (*(c+1) == 'a')	/* library */
			return(NULL);
		else
			*(c+1) = sub;
	} else
		return(NULL);
	return(x);
}

char *suf(file, suf)
	char *file, *suf;
{
	extern char *rindex();
	char *c, *x; 
	int flen, slen;

	flen = strlen(file);
	slen = strlen(suf);

	if (flen <= 0)
		return(NULL);
	if (rindex(file, '.') == NULL)
		return(NULL);
	if ((x = malloc(flen+slen+1)) == NULL) {
		perror("malloc");
		exit(1);
	}
	(void) strcpy(x, file);
	if ((c = rindex(x, '.')) != NULL) {
		*c = '\0';
		(void) strcat(x, suf);
	} else
		return(NULL);
	return(x);
}

char filtyp(file)
	char *file;
{
	extern char *rindex();
	char *c; 

	if (file == NULL)
		return('\0');
	if ((c = rindex(file, '.')) == NULL)
		return(-1);
	return(*(c+1));
}

Playery(mfile)
	char *mfile;
{
	char buf[BUFSIZ], *cfile, *flags, cpp[BUFSIZ];

	cfile = chsuf(mfile, DOTcFILE);
	if (integer)
		flags = "-i";
	else
		flags = " ";
	sprintf(cpp, "%s ", CPP);
	if (defcnt != 0) {
		int	i;
		for (i = 0; i < defcnt; i++)
			sprintf(cpp, "%s %s ", cpp, defstrs[i]);
	}
	/* /lib/cpp ${ofile} | ${PLAYERY} > ${rofile}.c */
	sprintf(buf, "%s %s | %s %s > %s", cpp, mfile, PLAYERY, flags, cfile);
	free(cfile);
	if (p_flag) 
		fprintf(stderr, "%s\n", buf);
	if (system(buf)) {
		fprintf(stderr, "Player failed on Playery\n");
		return(-1);
	}
	return(0);
}

cc(cfile)
	char *cfile;
{
	char buf[BUFSIZ];
	char *ofile;

	if (globs)
		sprintf(buf, "%s %s ", CC, globstr);
	else if (optimize)
		sprintf(buf, "%s %s ", CC, "-O");
	else
		sprintf(buf, "%s ", CC);
	if (defcnt != 0) {
		int	i;
		for (i = 0; i < defcnt; i++)
			sprintf(buf, "%s %s ", buf, defstrs[i]);
	}
	sprintf(buf, "%s %s", buf, cfile);
	sprintf(buf, "%s -o %s", buf, chsuf(cfile, DOToFILE));
	if (p_flag) 
		fprintf(stderr, "%s\n", buf);
	if (system(buf)) {
		fprintf(stderr, "Player failed on cc\n");
		return(-1);
	}
	return(0);
}

char *
Psymp(symbol)
	char *symbol;
{
	char	*c,
		*index();
	char	name[128];
	int	addr;
	char	type;

	/* is it a main(), m_start() or m_ter() symbol? */
	if ((c = index(symbol, '_')) != NULL) {
		if (!strcmp(c+1, MAIN)) {
			m_flag |= HASMAIN;
			return(NULL);
		} else if (!strcmp(c+1, M_START)) {
			m_flag |= MSTART;
			return(NULL);
		} else if (!strcmp(c+1, M_TER)) {
			m_flag |= MTER;
			return(NULL);
		}
	}
	/* is it a player initialization symbol? */
	if (sscanf(symbol, "%x %c %s", &addr, &type, name) != 3)
		return(NULL);
	/* is it a class template-stuffing routine? */
	if (!strncmp("_PC_", name, 4))
		return(c+1);
	/* is it a preset instance-stuffing routine? */
	if (!strncmp("_PP_", name, 4))
		return(c+1);
	return(NULL);
}


nm(file)
	char *file;
{
	FILE *pp, *popen();
	char tmp[BUFSIZ], *c;
	char *base, *sym;

	sprintf(tmp, "%s %s", NM, file);
	if (p_flag) 
		fprintf(stderr, "%s\n", tmp);
	pp = popen(tmp, "r");
	if (pp == NULL) {
		fprintf(stderr, "Player: nm: popen failed: %s\n", tmp); 
		return(-1);
	}
	while (fgets(tmp, BUFSIZ, pp) != NULL) {
		c = (char *) index(tmp, '\n'); 	/* zap return */ 
		if (c != NULL) 
			*c = NULL;
		if ((sym = Psymp(tmp)) != NULL)
			symcnt = strsav(&symary, sym);
	}
	pclose(pp);
	return(0);
}


char *
mkinit(file)
	char *file;
{
	int i, j, k;
	FILE *fp, *fopen();
	static char *buf[13];
	char *c;

	strcpy(buf, file);
	if ((fp = fopen(buf, "w")) == NULL) {
		perror("fopen");
		return(NULL);
	}
	if (integer)
		fprintf(fp, "# define P_INTEGER\n");
	else
		fprintf(fp, "# define P_FLOAT\n");
	fprintf(fp, "# include %s\n", FHEADER);
	if (!(m_flag & HASMAIN)) {
		if (integer) {
			fprintf(fp, "main()\n{\n");
			fprintf(fp, "\tPmain();\n");
		} else {
			fprintf(fp, "main(argc, argv)\n\tchar **argv;\n{\n");
			fprintf(fp, "\tPmain(argc, argv);\n");
		}
		fprintf(fp, "\tPexit(0);\n");
		fprintf(fp, "}\n");
	}
	fprintf(fp, "%s() \n{\n", QP);
	for (i = j = k = 0; i < symcnt; i++) {
		if (!strncmp(symary[i], "PC_", 3))
			fprintf(fp, "\textern void %s();\n", symary[i]);
		else
			fprintf(fp, "\textern Player *%s();\n", symary[i]);
	}
	fprintf(fp, "\n");
	for (i = j = k = 0; i < symcnt; i++) {
		if (!strncmp(symary[i], "PC_", 3))
			fprintf(fp, "\tPlayer *\tC%s = (Player *) calloc(1, sizeof(Player));\n", symary[i]);
# ifdef NOTDEF
		else
			fprintf(fp, "\tPlayer *\tP%s =  mk_Player(CPC%s);\n", 
				symary[i], &symary[i][2]);
# endif NOTDEF
	}
	for (i = j = k = 0; i < symcnt; i++) {
		if (!strncmp(symary[i], "PC_", 3)) {
			fprintf(fp, "\t%s(C%s);\n", symary[i], symary[i]);
			fprintf(fp, "\twhile (C%s->P_children--)\n",symary[i]);
			fprintf(fp, "\t\t(void) PP_%s(C%s);\n", 
				&symary[i][3], symary[i]);
		}
# ifdef NOTDEF
		else {
			fprintf(fp, "\t%s(P%s, CPC%s);\n", 
				symary[i], symary[i], &symary[i][2]);
		}
# endif NOTDEF
	}
	fprintf(fp, "}\n");
	if (!(m_flag & MSTART)) 
		fprintf(fp, "%s()\n{\n}\n", M_START);
	if (!(m_flag & MTER)) 
		fprintf(fp, "%s()\n{\n}\n", M_TER);
	fclose(fp);
	if (cc(buf)) 
		exit(1);
	return((char *) buf);
}

ld(initfile, ftl)
	char 	*initfile,	/* name of init file */
		**ftl;		/* names of files to be loaded */
{
	char buf[BUFSIZ], *ofile, *name;

	/* ld -X /lib/crt0.o vXX.o PlayerXXXXXX.o -l... -lc */
	(void) sprintf(buf, "%s ", LD);
	for ( ; *ftl != NULL; ftl++) {
		if ((name = chsuf(*ftl, DOToFILE)) != NULL)
			(void) strcat(buf, name);
		else
			(void) strcat(buf, *ftl);
		(void) strcat(buf, " ");
	}
	strcat(buf, initfile);
	if (globs && !integer)
		strcat(buf, SUN_gLIBS);
	else if (globs && integer)
		strcat(buf, SUN_giLIBS);
	else if (!globs && integer)
		strcat(buf, iLIBS);
	else if (!globs && !integer)
		strcat(buf, LIBS);
	if (p_flag) 
		fprintf(stderr, "%s\n", buf);
	if (system(buf)) {
		fprintf(stderr, "Player failed on ld\n");
		return(-1);
	}
	return(0);
}
