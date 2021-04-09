# makefile for all sources
# Victor Lazzarini, 2009

# set PREFIX to the install location prefix

PREFIX=/usr/local
PROGS =	\
	bicsf \
	cannon \
	cmusic \
	dgl \
	filter \
	fir \
	frm \
	gen \
	help \
	lpc \
	lprev \
	misc \
	noise \
	pvoc \
	rusty \
	show \
	sig \
	sndpath \
	srconv \
	stochist \
	zdelay \
	todac

MFLAGS = CFLAGS="-w -Wno-implicit-function-declaration -Wno-return-type -Wimplicit-int -I../../../include -I../../include -I../include" LFLAGS="-L../../lib"

all: progs install-local

progs:	lib/libcarl.a just_progs

just_progs:
	for file in ${PROGS} ; \
	do \
		echo $${file} ; cd $${file} ; make ${MFLAGS} ; cd ..; \
	done

lib/libcarl.a:
	cd lib		; make ${MFLAGS} install


install-local:  lib/libcarl.a
	for file in ${PROGS} ; \
	do \
		cd $${file} ; make ${MFLAGS} install ; cd ..;\
	done

install:
	sh install.sh ${PREFIX}

clean:	
	cd lib ; rm -f libbicsf.a ;make clean ;
	for file in ${PROGS} ; \
	do \
		cd $${file} ; make ${MFLAGS} clean ; cd ..; \
	done
	cd bin; rm -f *;

