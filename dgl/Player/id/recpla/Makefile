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
EXE	=	recpla
PLAYERS	=	recpla.o keytest.o M.o
CPLAYERS=	recpla.c keytest.c M.c
CFLAGS	=	-O
LIBDIR	=	/carl/lib
LIBS	=	${LIBDIR}/libiPlayer.a ../libstubs.a -lc
OBJS 	=	mpu_record.o\
		mpu_play.o\
		next_cmd.o\
		list_trk.o\
		init_trkp.o\
		setup_trk.o\
		dump_trk.o

.SUFFIXES:      .P
.P.c:           ;Player -P -i $*.P
.P.o:           ;Player -i -c -r $*.P

download: ${EXE}
	srec -e -l/dev/ttya -T2000 ${EXE}

${EXE}:	${PLAYERS} ${OBJS}
	Player -L -i -r ${PLAYERS}
	ld -N -T 2000 tmPlayer.o ${PLAYERS} ${OBJS} ${LIBS} -o ${EXE}

${CPLAYERS} ${OBJS}:	medit.h

srec:	${EXE}
	srec -e -T2000 ${EXE} > ${EXE}.sre

clean:
	rm -f *.o core a.out tmPlayer.[co] ${CPLAYERS}
