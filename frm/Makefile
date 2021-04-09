# cmusic Makefile
# prepared by Victor Lazzarini, 2009
DESTDIR =	../bin
CFLAGS = -O -I../include
LIBS =		../lib/libcarl.a -lm
PROGS =		chan click comb gain gaindelay hist ichan impulse \
		ochan offset para peak reverb rms window

all:	${PROGS}

chan: chan.o
	cc -O chan.o ${LIBS} -o chan
click: click.o
	cc -O click.o ${LIBS} -o click
comb: comb.o
	cc -O comb.o ${LIBS} -o comb
gain: gain.o
	cc -O gain.o ${LIBS} -o gain
gaindelay: gaindelay.o
	cc -O gaindelay.o ${LIBS} -o gaindelay
hist: hist.o
	cc -O hist.o ${LIBS} -o hist
ichan: ichan.o
	cc -O ichan.o ${LIBS} -o ichan
impulse: impulse.o
	cc -O impulse.o ${LIBS} -o impulse
ochan: ochan.o
	cc -O ochan.o ${LIBS} -o ochan
offset: offset.o
	cc -O offset.o ${LIBS} -o offset
para: para.o
	cc -O para.o ${LIBS} -o para
peak: peak.o
	cc -O peak.o ${LIBS} -o peak
reverb: reverb.o
	cc -O reverb.o ${LIBS} -o reverb
rms: rms.o
	cc -O rms.o ${LIBS} -o rms
spect: spect.o
	cc -O  spect.o ${LIBS} -I/usr/local/include -L/usr/local/lib -lplot -o spect
window: window.o
	cc -O window.o ${LIBS} -o window

install: ${PROGS}
	for file in ${PROGS} ; \
	do \
		echo install $${file} ${DESTDIR} ; \
		install $${file} ${DESTDIR} ; \
	done

clean:
	rm -f *.o a.out core
	rm -f chan click comb gain gaindelay hist ichan impulse \
	ochan offset para peak reverb rms spect window
