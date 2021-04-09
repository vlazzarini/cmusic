# cmusic Makefile
# prepared by Victor Lazzarini, 2009

DESTDIR =	../bin
LIBDIR =	../lib
DEBUG =
CFLAGS =	-O ${DEBUG} -I../include
LFLAGS =	${DEBUG}
OBJS =
LIBS =		 ../lib/libcarl.a ../lib/libieee.a -L/usr/local/gfortran/lib -lgfortran   -lm

all:		convert srconv

convert:	convert.o ${OBJS}
		${CC} convert.o ${OBJS} ${LIBS} -o convert

srconv:		srconv.o ${OBJS}
		${CC} srconv.o ${OBJS} ${LIBS} -o srconv

install:	all
		install convert ${DESTDIR}
		install srconv ${DESTDIR}
		-if test ! -d ${LIBDIR}/srconv ; \
		then \
			mkdir ${LIBDIR}/srconv ; \
			cp flts/*.flt ${LIBDIR}/srconv ; \
		fi

clean:
		rm -f *.o convert
		rm -f *.o srconv
