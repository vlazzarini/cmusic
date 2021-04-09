include(/usr/local/include/carl/config.m4)dnl

CFLAGS =	-g
LIBDIR	=	/carl/lib
.SUFFIXES:      .P

.P.c:           ;Player -P -i $*.P
.P.o:           ;Player -i -L -r $*.P

lag:	${p}.o load

load:
	Player -i -L -r ${p}.o
	ld -N -T 2000 tmPlayer.o ${p}.o ${LIBDIR}/libiPlayer.a libstubs.a -lc
	srec -e -l/dev/ttys0 -T2000 a.out

srec:
	srec -b9600 -e -l/dev/ttys0 -T2000 a.out
