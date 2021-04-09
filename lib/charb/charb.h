# ifdef debug
# define CBINC	1
# else debug
# define CBINC	16
# endif debug

/*
 * macros to get at
 * parts of a charb
 */
# define cbbuf(cb)		((cb)->c_buf)
# define cbmax(cb)		((cb)->c_max)

typedef struct {
	unsigned short c_max;
	char *c_buf;
} charb;

extern charb *cbrealloc();
extern charb *cballoc();
extern charb *cbncat();
extern charb *cbcat();
extern charb *cbncpy();
extern charb *cbcpy();
extern charb *makcb();
