# cmusic Makefile
# prepared by Victor Lazzarini, 2009

DESTDIR =	../bin
CFLAGS =	-g -I../include
LIBS =		../lib/libcarl.a 

ALLPROGS	= readheader stripheader

all:		${ALLPROGS} 

readheader:	readheader.o ${OBJS}
		${CC} ${CFLAGS} readheader.o ${OBJS} ${LIBS} -o readheader

stripheader:	stripheader.o ${OBJS}
		${CC} ${CFLAGS} stripheader.o ${OBJS} ${LIBS} -o stripheader

install:
		cp ${ALLPROGS} ${DESTDIR}

clean:
	rm -f *.o a.out core ${ALLPROGS}
