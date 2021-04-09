

PROG =		tosf

INCLUDE =	../include
DESTDIR =	../../bin
CFLAGS =	-O -I${INCLUDE} -I../../include
LIBS =		-L../../lib -lcarl -lbicsf -lm
OBJS =		rhead.o ftof.o stos.c ftos.o stof.o
DBUG =

${PROG}:	${PROG}.o ${OBJS} ${DBUG}
		${CC} ${CFLAGS} ${PROG}.o ${DBUG} ${OBJS} ${LIBS} -o ${PROG}
		cd ..

install:	${PROG}
		cp ${PROG} ${DESTDIR}
		cd ..

clean:
		rm -f core a.out *.o ${PROG} install
