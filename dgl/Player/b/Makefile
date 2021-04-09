# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG =		Player

DESTDIR =	../../../bin
CFLAGS =	-g -I../../../include
LIBS =
OBJS =		y.tab.o clexer.o yylex.o ymain.o sym.o hasher.o dvrt.o \
		globs.o strsav.o

${PROG}y: ${OBJS}
	cc ${CFLAGS} ${OBJS} -L../../../lib -lcarl -o ${PROG}y 

y.tab.o:	y.tab.c

y.tab.c: 	Player.y
		yacc -v -d Player.y
		sed "/# line/d" y.tab.c > y.tab.x
		mv y.tab.x y.tab.c


install:	${PROG}y
		cp ${PROG}y ${DESTDIR}

clean:
		rm -f *.o core a.out ${PROG}y
