# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG =		noise

DESTDIR =	../bin
CFLAGS =	-O -I../include
LIBS =		../lib/libcarl.a -lm
OBJS =		frand.o irand.o onefrand.o brand.o xrand.o

${PROG}:	${PROG}.o ${OBJS}
		${CC} ${PROG}.o ${OBJS} ${LIBS} -o ${PROG}

install:	${PROG}
		cp ${PROG} ${DESTDIR}

clean:
		rm -f *.o ${PROG}
