include(/usr/local/include/carl/config.m4)dnl

PROG =		casslab

DESTDIR =	m4DESTDIR
CFLAGS =	-O
LIBS =
OBJS =

${PROG}:	${PROG}.o ${OBJS}
		${CC} ${PROG}.o ${OBJS} ${LIBS} -o ${PROG}

install:	${PROG}
		cp ${PROG} ${DESTDIR}

clean:
		rm -f *.o core a.out ${PROG}
