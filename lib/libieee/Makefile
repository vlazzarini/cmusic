# cmusic Makefile
# prepared by Victor Lazzarini, 2009
# makefile for libieee

# Requires gfortran to be installed 


LIBDIR	=	..
FFLAGS =	-O -w -std=legacy
FC =            gfortran
PROG =		libieee
OBJS =		d1mach.o eqfir.o fast.o fourea.o fwfir.o \
		i1mach.o lpcsub.o lptrn.o mdecint.o pmpse.o r1mach.o \
		srconv.o uni.o window.o intfil.o mxfft.o fft842.o


# the following routine has been lost from this directory
# but used to be included in OBJS
#		fsst.o 

.f.o:
		${FC} ${FFLAGS} -c $*.f
		-ld  -r $*.o
		mv a.out $*.o

${PROG}: 	${OBJS}
		rm -f ../libieee.a
		ar cru ../libieee.a ${OBJS}

clean:
		rm -f ${OBJS}


