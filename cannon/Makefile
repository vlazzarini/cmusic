# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG =		cannon

DESTDIR =	../bin
CFLAGS =	-O -I../include
LIBS =	        -lm -L../lib -lcarl -lm
OBJS =

${PROG}:	${PROG}.o ${OBJS}
		${CC} ${PROG}.o ${OBJS} ${LIBS} -o ${PROG}

install:	${PROG}
		cp cannon ${DESTDIR}

clean:
		rm -f *.o cannon
