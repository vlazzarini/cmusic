# cmusic Makefile
# prepared by Victor Lazzarini, 2009

# This links to libiee, which requires gfortran to be
# installed 
# gfortran lib location defaults to /usr/local/gfortran/lib

GFORTLIB = /usr/local/gfortran/lib
DESTDIR =	../bin
CFLAGS =	-O -I../include
FFLAGS =	-w
LIBS =		../lib/libcarl.a ../lib/libieee.a -L${GFORTLIB} -lgfortran  -lm
FC = gfortran

all: 	fastfir    fir

fir: fir.f firsub.f
	gfortran -c -o fir.o fir.f
	gfortran --fixed-line-length-180 -c -o firsub.o firsub.f
	gfortran fir.o firsub.o ../lib/libieee.a -o fir

fastfir: fastfir.o
	cc -O fastfir.o ${LIBS} -o fastfir

install: all
	install fir ${DESTDIR}
	install fastfir ${DESTDIR}

clean:
	rm -f *.o a.out core
	rm -f fir fastfir 

