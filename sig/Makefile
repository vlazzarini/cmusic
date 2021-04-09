# cmusic Makefile
# prepared by Victor Lazzarini, 2009

# These programs require gfortran to be installed
# as they link to libgfortran.*
# We assume it to be installed in the default location(s)
# (ie. /usr/local/gfortran/lib or the default lib directories)

DESTDIR =	../bin
CFLAGS =	-O -I../include
LIBS =		../lib/libcarl.a ../lib/libieee.a -L/usr/local/gfortran/lib -lgfortran  -lm
OBJS =

all:		derivative energy envanal envelope fftanal janus \
		glitch atob btoa median pitch wave

atob:	atob.o ${OBJS}
		${CC} atob.o ${OBJS} ${LIBS} -o atob

btoa:	btoa.o ${OBJS}
		${CC} btoa.o ${OBJS} ${LIBS} -o btoa

derivative:	derivative.o ${OBJS}
		${CC} derivative.o ${OBJS} ${LIBS} -o derivative

energy:		energy.o ${OBJS}
		${CC} energy.o ${OBJS} ${LIBS} -o energy

envanal:	envanal.o ${OBJS}
		${CC} envanal.o ${OBJS} ${LIBS} -o envanal

envelope:	envelope.o ${OBJS}
		${CC} envelope.o ${OBJS} ${LIBS} -o envelope

fftanal:	fftanal.o ${OBJS}
		${CC} fftanal.o ${OBJS} ${LIBS} -o fftanal

janus:		janus.o ${OBJS}
		${CC} ${CFLAGS} janus.o ${OBJS} ${LIBS} -o janus

glitch:		glitch.o ${OBJS}
		${CC} glitch.o ${OBJS} ${LIBS} -o glitch

median:		median.o ${OBJS}
		${CC} median.o ${OBJS} ${LIBS} -o median

pitch:		pitch.o ${OBJS}
		${CC} pitch.o ${OBJS} ${LIBS} -o pitch

wave:		wave.o ${OBJS}
		${CC} wave.o ${OBJS} ${LIBS} -o wave


install:	all
		install atob ${DESTDIR}
		install btoa ${DESTDIR}
		install derivative ${DESTDIR}
		install energy ${DESTDIR}
		install envanal ${DESTDIR}
		install envelope ${DESTDIR}
		install fftanal ${DESTDIR}
		install janus ${DESTDIR}
		install glitch ${DESTDIR}
		install median ${DESTDIR}
		install pitch ${DESTDIR}

clean:
		rm -f *.o core derivative energy envanal envelope fftanal \
		janus atob btoa glitch median pitch 
