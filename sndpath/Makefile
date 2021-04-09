# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG =		sndpath

DESTDIR =	../bin
CFLAGS =	-g -I../include
LIBS =		../lib/libcarl.a -lm -lcurses
OBJS =

all:		sndpath

sndpath:	sndpath.o nodfun.o spl.o
		${CC} sndpath.o nodfun.o spl.o ${OBJS} ${LIBS} -o ${PROG}

install:	all
		cp sndpath ${DESTDIR}

clean:
		rm -f *.o sndpath 

sndpath.o nodfun.o spl.o: list.h
