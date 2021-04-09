# cmusic Makefile
# prepared by Victor Lazzarini, 2009

# These programs require gfortran to be installed
# as they link to libgfortran.*
# We assume it to be installed in the default location(s)
# (ie. /usr/local/gfortran/lib or the default lib directories)

PROG =		pvoc

DESTDIR =	../bin
CFLAGS =	-O -I../include
LIBS =		../lib/libcarl.a  ../lib/libieee.a -L/usr/local/gfortran/lib -lgfortran  -lm
OBJS =		kaiser.f
FC = gfortran

${PROG}:	${PROG}.o ${OBJS}
		${FC} kaiser.f -c -o kaiser.o
		${CC} ${CFLAGS} ${PROG}.o kaiser.o ${LIBS} -o ${PROG}

install:	${PROG}
		install ${PROG} ${DESTDIR}

clean:
		rm -f *.o ${PROG}

