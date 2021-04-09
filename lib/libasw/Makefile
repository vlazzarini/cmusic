# cmusic Makefile
# prepared by Victor Lazzarini, 2009


CFLAGS =	-O

PROG =		libasw
OBJS =		openasw.o getplace.o connasw.o disconnasw.o resetasw.o \
		initasw.o

.c.o:
		${CC} ${CFLAGS} -c $*.c
		-ld -r $*.o
		mv a.out $*.o

${PROG}: 	${OBJS}
		ar r ../libcarl.a ${OBJS}

clean:
		rm -f ${OBJS}


