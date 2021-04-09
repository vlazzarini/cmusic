include(/usr/local/include/carl/config.m4)dnl
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
