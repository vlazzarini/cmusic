include(/usr/local/include/carl/config.m4)dnl
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
