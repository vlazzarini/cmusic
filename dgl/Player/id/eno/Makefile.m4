include(/usr/local/include/carl/config.m4)dnl
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
