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

PROG =		libiPlayer

DESTDIR =	//usr/local/lib/carl//bin
LIBDIR =	/usr/local/lib/carl//lib
CFLAGS =	-g -DP_INTEGER
OBJS = 		Caller.o \
		Instances.o\
		Join.o \
		Kill.o \
		Kill_test.o \
		New_player.o \
		Pexit.o \
		Pinit.o \
		Player_status.o \
		Pmain.o \
		Print_stat.o \
		Psleep.o \
		Rest.o \
		Resting.o \
		Set_beg_time.o \
		Set_dur_time.o \
		Set_p.o \
		Set_pp.o \
		Set_ps.o \
		Silent.o \
		Start.o \
		Stop.o \
		Unrest.o \
		Unsilent.o \
		Wait_for.o \
		Wait_until.o \
		check_waits.o \
		ckmaxp.o \
		garbage.o \
		get_templ.o \
		globs.o \
		malerr.o \
		mk_Player.o \
		prq.o \
		ptimer.o\
		ptm.o \
		run_arbiter.o \
		sched.o \
		symtab.o

OUT =		malloc.o calloc.o ubrk.o

DBUG =

do_it:		/usr/local/include/carl/Player.h ${PROG}

/usr/local/include/carl/Player.h:	../c/Player.h
		cp ../c/Player.h /usr/local/include/carl

${PROG}: 	${DBUG} ${OBJS}
		ar uv ${PROG}.a ${DBUG} ${OBJS}
		ranlib ${PROG}.a

install:
		rm -f ${LIBDIR}/${PROG}.a
		ln ${PROG}.a ${LIBDIR}/${PROG}.a

clean:
		rm -f ${PROG}.a ${DBUG} ${OBJS} a.out core


${DBUG} ${OBJS}: /usr/local/include/carl/Player.h 

Pitches.o:	Scale.h

pub:
		rm -f /usr/local/src/carl/src/lib/${PROG}/*
		cp Makefile.m4 *.[cfpshly] /usr/local/src/carl/src/lib/${PROG}
