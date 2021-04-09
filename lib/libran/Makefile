# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG		= libran

CFLAGS =	-O -I../../include

OBJS =		frand.o arcsin.o cauchy.o expn.o gamma.o gauss.o hyper.o lin.o \
		logist.o beta.o plapla.o frand.o onefrand.o corrand.o \
		randfi.o randfc.o

.c.o:
		${CC} ${CFLAGS} -c $*.c
		-ld -r $*.o
		mv a.out $*.o

${PROG}: 	${OBJS}
		ar r ../libcarl.a ${OBJS}

alone: 		${OBJS}
		ar cr ${PROG} ${OBJS}
		ranlib ${PROG}
clean:
		rm -f ${OBJS}


