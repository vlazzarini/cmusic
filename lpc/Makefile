# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG =		lpc

DESTDIR =	../bin
CFLAGS =	-O -I../include 
LIBS =		-L../lib -lcarl -lieee -L/usr/local/gfortran/lib -lgfortran -lm
OBJS =

${PROG}:	${PROG}.o ${OBJS}
		${CC} ${PROG}.o ${OBJS} ${LIBS} -o ${PROG}

install:	${PROG}
		install ${PROG} ${DESTDIR}

clean:
		rm -f *.o ${PROG}
