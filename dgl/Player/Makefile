# cmusic Makefile
# prepared by Victor Lazzarini, 2009


PROG =		prog

DESTDIR =	../../bin
INCLUDE	=	../../include/carl
CFLAGS =	-O -DP_FLOAT
LIBS =		../../lib/libcarl.a 
OBJS =

all: ${INCLUDE}/Player.h Playery Player putP libPlayer



libPlayer:
	cd c; make

libiPlayer:
	cd ic; make

Playery:
	cd b; make

${INCLUDE}/Player.h: c/Player.h
	cd c; cp Player.h ${INCLUDE}

Player:
	cd a; make 

putP:
	cd a; make install
	cd b; make install

install:
	cd a; make install
	cd b; make install
	cd c; make install

clean:
	cd a; make clean
	cd b; make clean
	cd c; make clean
	cd d; make clean
	cd doc; make clean
	rm -f errs

test:
	cd d; make >& /tmp/P_test; diff /tmp/P_test P_test
