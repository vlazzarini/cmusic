include(/usr/local/include/carl/config.m4)dnl
PROG	=	libxcmpu
CFLAGS	=	-O
MPU	=	mpuio.o mpu_init.o mpu_asint.o mpu_cint.o watch_pedal.o \
		libm68.o ckmem.o trap_init.o trap_err.o pedal.o

${PROG}:	${MPU}
	ar cr ${PROG}.a ${MPU}

clean:
	rm -f *.o core a.out ${PROG}.a

install:
	mv ${PROG}.a ..
	ranlib ../${PROG}.a
