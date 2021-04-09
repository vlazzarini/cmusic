# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG =		zdelay

DESTDIR =	../bin
CFLAGS =	-O -I../include
LIBS =		../lib/libcarl.a -lm
OBJS =		mzdelay.o 

${PROG}:	${PROG}.o ${OBJS}
		${CC} ${PROG}.o ${OBJS} ${LIBS} -o ${PROG}

install:	${PROG}
		cp ${PROG} ${DESTDIR}

clean:
		rm -f *.o ${PROG}
