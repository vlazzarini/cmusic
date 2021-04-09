# cmusic Makefile
# prepared by Victor Lazzarini, 2009
PROG =		Player

DESTDIR =	../../../bin
CFLAGS =	-g -I../../../include
LIBS =		-L../../../lib -lcarl -lbicsf
OBJS =		strsav.o

${PROG}:	${PROG}.o ${OBJS}
		${CC}  ${PROG}.o ${OBJS} ${CFLAGS} ${LIBS} -o ${PROG}

${PROG}.o:	${PROG}.c
		${CC} -c -DPLAYERY=\"${DESTDIR}/Playery\" ${PROG}.c ${CFLAGS}


install:	${PROG}
		cp ${PROG} ${DESTDIR}

clean:
		rm -f *.o core a.out ${PROG}
