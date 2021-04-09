# cmusic Makefile
# prepared by Victor Lazzarini, 2009

DESTDIR =	../bin
CFLAGS =	-O -I../include
LIBS =		../lib/libcarl.a -lm
OBJS =

all:		chubby cspline gen0 gen1 gen2 gen3 gen4 gen5 gen6 genraw quad \
			step shepenv

shepenv:	shepenv.o ${OBJS}
		${CC} shepenv.o ${OBJS} ${LIBS} -o shepenv

chubby:		chubby.o ${OBJS}
		${CC} chubby.o ${OBJS} ${LIBS} -o chubby

cspline:	cspline.o ${OBJS}
		${CC} cspline.o ${OBJS} ${LIBS} -o cspline

gen0:	gen0.o ${OBJS}
		${CC} gen0.o ${OBJS} ${LIBS} -o gen0

gen1:	gen1.o ${OBJS}
		${CC} gen1.o ${OBJS} ${LIBS} -o gen1

gen2:	gen2.o ${OBJS}
		${CC} gen2.o ${OBJS} ${LIBS} -o gen2

gen3:	gen3.o ${OBJS}
		${CC} gen3.o ${OBJS} ${LIBS} -o gen3

gen4:	gen4.o ${OBJS}
		${CC} gen4.o ${OBJS} ${LIBS} -o gen4

gen5:	gen5.o ${OBJS}
		${CC} gen5.o ${OBJS} ${LIBS} -o gen5

gen6:	gen6.o ${OBJS}
		${CC} gen6.o ${OBJS} ${LIBS} -o gen6

genraw:		genraw.o ${OBJS}
		${CC} genraw.o ${OBJS} ${LIBS} -o genraw

quad:		quad.o nodfun.o ${OBJS}
		${CC} quad.o nodfun.o ${OBJS} ${LIBS} -o quad

step:		step.o ${OBJS}
		${CC} step.o ${OBJS} ${LIBS} -o step

all:		chubby cspline gen0 gen1 gen2 gen3 gen4 gen5 gen6 genraw quad \
			step shepenv
install:	all strip
		cp chubby ${DESTDIR}
		cp cspline ${DESTDIR}
		cp gen0 ${DESTDIR}
		cp gen1 ${DESTDIR}
		cp gen2 ${DESTDIR}
		cp gen3 ${DESTDIR}
		cp gen4 ${DESTDIR}
		cp gen5 ${DESTDIR}
		cp gen6 ${DESTDIR}
		cp genraw ${DESTDIR}
		cp quad ${DESTDIR}
		cp step ${DESTDIR}
		cp shepenv ${DESTDIR}

strip:
		strip chubby 
		strip cspline 
		strip gen0 
		strip gen1 
		strip gen2 
		strip gen3 
		strip gen4 
		strip gen5 
		strip gen6 
		strip genraw 
		strip quad 
		strip step 
		strip shepenv 

clean:
		rm -f *.o a.out core
		rm -f chubby cspline gen0 gen1 gen2 gen3 gen4 gen5 gen6 genraw \
			quad step shepenv
