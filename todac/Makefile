# cmusic Makefile
# prepared by Victor Lazzarini, 2009
# todac utility


PROG =		todac

DESTDIR =	../bin
CFLAGS =	-O -I/usr/local/include
LIBS =	        -lm -L/usr/local/lib -lportaudio 
OBJS =

${PROG}:	${PROG}.o ${OBJS}
		${CC} ${PROG}.o ${OBJS} ${LIBS} -o ${PROG}

install:
		cp todac ${DESTDIR}

clean:
		rm -f *.o todac
