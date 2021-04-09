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
PROG	=	eno2
CFLAGS	=	-O
LIBDIR	=	/carl/lib
LIBS	=	${LIBDIR}/libiPlayer.a ../libstubs.a ../libxcmpu.a -lc
OBJS 	=	midi_gen.o Rand.o

.SUFFIXES:      .P
.P.c:           ;Player -P -i $*.P
.P.o:           ;Player -i -L -r $*.P

download: ${PROG}
	srec -e -l -T2000 ${PROG}

${PROG}:	${PROG}.o ${OBJS}
	ld -N -T 2000 ${PROG}.o tmPlayer.o ${OBJS} ${LIBS} -o ${PROG}

srec:	${PROG}
	srec -e -T2000 ${PROG} > ${PROG}.sre

clean:
	rm -f *.o core a.out ${PROG} ${PROG}.sre ${PROG}.c
