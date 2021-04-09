include(/usr/local/include/carl/config.m4)dnl

LIBDIR =	m4LIBDIR
CFLAGS =	-O

PROG =		libasw
OBJS =		openasw.o getplace.o connasw.o disconnasw.o resetasw.o \
		initasw.o

.c.o:
		${CC} ${CFLAGS} -c $*.c
		-ld -x -r $*.o
		mv a.out $*.o

${PROG}: 	${OBJS}
		ar r ../libcarl.a ${OBJS}

clean:
		rm -f ${OBJS}

pub:
		cp * m4SRCDIR/lib/${PROG}
