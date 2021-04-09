# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG =		libPlayer
CFLAGS =	-g -DP_FLOAT -I../../../include
.SUFFIXES:      .P
.P.o:           ;Player -P $*.P && cc -c ${CFLAGS} $*.c

DESTDIR =	../../../bin
LIBDIR =	../../../lib
OBJS =		Abs_time.o\
		Alias.o\
		Beg_time.o\
		Caller.o\
		Direction.o\
		Dur_time.o\
		End_time.o\
		Expr.o\
		Exprs.o\
		Find_measure.o\
		Func.o\
		Func_length.o\
		Gen.o\
		Get_keysig.o\
		Get_list.o\
		Get_metronome.o\
		Get_pname.o\
		Get_paddr.o\
		Get_timesig.o\
		Hipn.o\
		Index.o\
		Instances.o\
		Join.o\
		Key.o\
		Keys.o\
		Kill.o\
		Kill_test.o\
		Motive.o\
		New_player.o\
		Octave.o\
		Oct_pt.o\
		Oct_pts.o\
		P.o\
		Peekcl.o\
		Pexit.o\
		Pitch.o\
		Pitches.o\
		Player_status.o\
		Pmain.o\
		Pp.o\
		Print_stat.o\
		Ps.o\
		Psleep.o\
		Read_func.o\
		Read_list.o\
		Read_lists.o\
		Rel_func.o\
		Rel_time.o\
		Rest.o\
		Resting.o\
		Rhythm.o\
		Rhythms.o\
		Set_beg_time.o\
		Set_dur_time.o\
		Set_p.o\
		Set_pi.o\
		Set_pp.o\
		Set_ps.o\
		Setup_p.o\
		Setup_pp.o\
		Setup_ps.o\
		Silent.o\
		Start.o\
		Stop.o\
		Strings.o\
		SubPname.o\
		Tempo.o\
		Tmap.o\
		Unrest.o\
		Unsilent.o\
		Unwait_until.o\
		Wait_for.o\
		Wait_until.o\
		add_player.o\
		check_waits.o\
		ckmaxp.o\
		clist_direct.o\
		cmusout.o\
		cpy_args.o\
		cyclist.o\
		do_rhy.o\
		garbage.o\
		get_templ.o\
		globs.o\
		hi_p_ck.o\
		interplist.o\
		keysig.o\
		malerr.o\
		metronome.o\
		mk_Player.o\
		parse_flags.o\
		pc_oct.o\
		pexprerr.o\
		poll_waits.o\
		read_notes.o\
		read_stdin.o\
		resize_p.o\
		run_arbiter.o\
		scanargs.o\
		sched.o\
		symtab.o\
		symtab_stat.o\
		timesig.o

do_it:		../../../include/carl/Player.h ${PROG}

../../../include/carl/Player.h:	Player.h
		cp Player.h ../../../include/carl

${OBJS}:	../../../include/carl/Player.h 

${PROG}: 	${OBJS}
		ar uvrc ${PROG}.a ${DBUG} ${OBJS}

install:
		cp ${PROG}.a ${LIBDIR}/${PROG}.a
		ranlib ${LIBDIR}/${PROG}.a

clean:
		rm -f ${PROG}.a ${DBUG} ${OBJS} *.3 a.out core

Pitches.o:	Scale.h

#.c.o:
#		${CC} ${CFLAGS} -c $*.c
#		-ld -x -r $*.o
#		mv a.out $*.o


