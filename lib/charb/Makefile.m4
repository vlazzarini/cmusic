include(/usr/local/include/carl/config.m4)dnl

DESTDIR =	m4DESTDIR
CFLAGS =	-O
LIBS =		m4CARLLIBS m4FORTLIBS m4MATHLIB
OBJS =

prog:		prog.o ${OBJS}
		${CC} ${OBJS} ${LIBS} -o prog

install:	prog
		cp prog ${DESTDIR}

clean:
		rm -f *.o prog
