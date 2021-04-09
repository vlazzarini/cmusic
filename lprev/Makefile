# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG =		lprev

DESTDIR =	../bin
CFLAGS =	-O -I../include
LIBS =		../lib/libcarl.a -lm
OBJS =		tap.o cflt.o getibuf.o apass.o revconf.o

${PROG}:	${PROG}.o ${OBJS}
		${CC} ${PROG}.o ${OBJS} ${LIBS} -o ${PROG}

install:	${PROG}
		install ${PROG} ${DESTDIR}

clean:
		rm -f *.o ${PROG}

${OBJS}:	lprev.h
tap.c:		tap.h
cflt.c:		cflt.h
apass.c:	apass.h
