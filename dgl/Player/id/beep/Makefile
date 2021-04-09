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
PROG	=	beep
CFLAGS	=	-g
LIBDIR	=	/carl/lib
LIBS	=	${LIBDIR}/libiPlayer.a ../libstubs.a -lc
OBJS 	=	midi_gen.o
MPU	=	mpuio.o mpu_init.o mpu_asint.o mpu_cint.o watch_pedal.o \
		libm68.o ckmem.o trap_init.o trap_err.o pedal.o

.SUFFIXES:      .P
.P.c:           ;Player -P -i $*.P
.P.o:           ;Player -i -L -r $*.P

download: ${PROG}
	srec -e -l -T2000 ${PROG}

${PROG}:	${PROG}.o ${OBJS} ${MPU}
	ld -N -T 2000 ${PROG}.o tmPlayer.o ${OBJS} ${MPU} ${LIBS} -o ${PROG}

srec:	${PROG}
	srec -e -T2000 ${PROG} > ${PROG}.sre

clean:
	rm -f *.o core a.out ${PROG} ${PROG}.sre
