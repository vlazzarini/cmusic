# config.m4	1.4	(CMIL)	9/19/88	17:17:26

# flavor of UNIX in use
# type of CPU
# paths to libraries
# SDY define(m4CARLLIBPATH, m4LIBDIR/libcarl.a m4LIBDIR/libieee.a)dnl
# SDY define(m4CARLLIBS, m4LIBDIR/libcarl.a m4LIBDIR/libieee.a)dnl
# save for now SDY define(m4SFLIBS, m4LIBDIR/libsf.a)dnl
# Fortran libraries must be given in the following order for SUN3.x
# final resting place for installed software
# directory for include files
# where help files will live
# directory for tutorials 
# location of sources at CARL
# path to manual pages
# path to sys include directory
# path to vaxuba include directory
# dac/adc converters to use: one of ds (DSC200), ai (Digisound-16)
# sound filesystem to use: may be either csound or bicsf

DESTDIR =	//usr/local/lib/carl//bin
CFLAGS =	-O
LIBS =		/usr/local/lib/carl//lib/libcarl.a -lF77 -lI77  -lm
OBJS =

prog:		prog.o ${OBJS}
		${CC} ${OBJS} ${LIBS} -o prog

install:	prog
		cp prog ${DESTDIR}

clean:
		rm -f *.o prog
