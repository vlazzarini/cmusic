# cmusic Makefile
# prepared by Victor Lazzarini, 2009
# makefile for libfrm component of libcarla

CFLAGS =	-O -I../../include

PROG =		libfrm
OBJS =		asciifile.o expr.o polish.o \
		trans.o

.c.o:
		${CC} ${CFLAGS} -c $*.c
		-ld  -r $*.o
		mv a.out $*.o

${PROG}: 	${OBJS}
		ar r ../libcarl.a ${OBJS}

clean:
		rm -f ${OBJS}


