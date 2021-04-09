include(/usr/local/include/carl/config.m4)dnl

PROG =		libiPlayer

DESTDIR =	m4DESTDIR
LIBDIR =	m4LIBDIR
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

do_it:		m4INCLUDE/Player.h ${PROG}

m4INCLUDE/Player.h:	../c/Player.h
		cp ../c/Player.h m4INCLUDE

${PROG}: 	${DBUG} ${OBJS}
		ar uv ${PROG}.a ${DBUG} ${OBJS}
		ranlib ${PROG}.a

install:
		rm -f ${LIBDIR}/${PROG}.a
		ln ${PROG}.a ${LIBDIR}/${PROG}.a

clean:
		rm -f ${PROG}.a ${DBUG} ${OBJS} a.out core


${DBUG} ${OBJS}: m4INCLUDE/Player.h 

Pitches.o:	Scale.h

pub:
		rm -f m4SRCDIR/lib/${PROG}/*
		cp Makefile.m4 *.[cfpshly] m4SRCDIR/lib/${PROG}
