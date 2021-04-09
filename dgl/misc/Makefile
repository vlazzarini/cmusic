# cmusic Makefile
# prepared by Victor Lazzarini, 2009

DESTDIR =	../../bin
CFLAGS =	-O -I../../include
LIBS =		../../lib/libcarl.a -lm
OBJS = 		equtemp cexpr pianoroll sched r18 xform fc cmt ucmt \
	ispell lookfor m4n nonzero rect thresh channel yadc

all:	${OBJS}

signum: signum.o setchan.o
	${CC} signum.o ${LIBS} -o signum

channel: channel.o setchan.o
	${CC} channel.o setchan.o ${LIBS} -o channel

janus: janus.o setchan.o
	${CC} janus.o ${LIBS} -o janus

thresh: thresh.o
	${CC} thresh.o ${LIBS} -o thresh

rect: rect.o
	${CC} rect.o ${LIBS} -o rect

nonzero: nonzero.o
	${CC} nonzero.o ${LIBS} -o nonzero

m4n: m4n.o
	${CC} m4n.o ${LIBS} -o m4n

lookfor: lookfor.o
	${CC} lookfor.o ${LIBS} -o lookfor

ispell: ispell.o
	${CC} ispell.o ${LIBS} -o ispell

equtemp: equtemp.o
	${CC} equtemp.o ${LIBS} -o equtemp

cexpr: cexpr.o
	${CC} cexpr.o ${LIBS} -o cexpr

pianoroll: pianoroll.o
	${CC} pianoroll.o ${LIBS} -o pianoroll

sched: sched.o
	${CC} sched.o ${LIBS} -o sched

r18: r18.o
	${CC} r18.o ${LIBS} -o r18

xform: xform.o
	${CC} xform.o ${LIBS} -o xform

fc: fc.o
	${CC} fc.o ${LIBS} -o fc

cmt: cmt.o
	${CC} cmt.o ${LIBS} -o cmt

ucmt: ucmt.o
	${CC} ucmt.o ${LIBS} -o ucmt

yadc:	yadc.o
	${CC} yadc.o ${LIBS} -o yadc

yadc.c:	yadc.y


install: all
	mv equtemp ${DESTDIR}
	mv cexpr ${DESTDIR}
	mv pianoroll ${DESTDIR}
	mv sched ${DESTDIR}
	mv r18 ${DESTDIR}
	mv xform ${DESTDIR}
	mv fc ${DESTDIR}
	mv cmt ${DESTDIR}
	mv ucmt ${DESTDIR}
	mv ispell ${DESTDIR}
	mv lookfor ${DESTDIR}
	mv m4n ${DESTDIR}
	mv nonzero ${DESTDIR}
	mv rect ${DESTDIR}
	mv thresh ${DESTDIR}
	mv yadc ${DESTDIR}

clean:
		rm -f *.o core a.out ${OBJS}
