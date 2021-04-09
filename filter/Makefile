# cmusic Makefile
# prepared by Victor Lazzarini, 2009

DESTDIR =	../bin
CFLAGS =	-O -I../include
LIBS =		../lib/libcarl.a -lm

# SDY convolve needs ieee
# all: 	filter convolve
all: 	filter convolve

filter: filter.o
	cc -O filter.o ${LIBS} -o filter

convolve: convolve.o
	cc -O convolve.o ${LIBS} -L/usr/local/gfortran/lib -lgfortran ../lib/libieee.a -lm -o convolve

install: all
	install filter ${DESTDIR}
	install convolve ${DESTDIR}

clean:
	rm -f *.o a.out core
	rm -f filter convolve 
