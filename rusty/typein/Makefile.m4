include(/usr/local/include/carl/config.m4)dnl

DESTDIR =	m4DESTDIR
CFLAGS =	-O

all:	typein rtypein

typein:	typein.o
		${CC} ${CFLAGS} typein.o -o typein

rtypein:	rtypein.o 
		${CC} ${CFLAGS} rtypein.o -o rtypein

install:	typein rtypein
		install typein ${DESTDIR}
		install rtypein ${DESTDIR}

clean:
		rm -f *.o core a.out typein rtypein
