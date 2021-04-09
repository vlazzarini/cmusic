# config.m4	1.4	(CMIL)	9/19/88	17:17:26

# flavor of UNIX in use
# type of CPU
# paths to libraries
# SDY define(m4CARLLIBPATH, m4LIBDIR/libcarl.a m4LIBDIR/libieee.a)dnl
# SDY define(m4CARLLIBS, m4LIBDIR/libcarl.a m4LIBDIR/libieee.a)dnl
# save for now SDY define(m4SFLIBS, m4LIBDIR/libsf.a)dnl
# Fortran libraries must be given in the following order for SUN3.x
# final resting place for installed software
# directory for include files
# where help files will live
# directory for tutorials 
# location of sources at CARL
# path to manual pages
# path to sys include directory
# path to vaxuba include directory
# dac/adc converters to use: one of ds (DSC200), ai (Digisound-16)
# sound filesystem to use: may be either csound or bicsf

FILES =	../../include/carl/sfheader.h\
	../../include/carl/crack.h\
	../../include/carl/libsf.h\
	../../include/carl/paths.h\
	../../include/carl/argscan.h\
	../../include/carl/defaults.h\
	../../include/carl/ircam.h\
	../../include/carl/filesf.h\
	../../include/carl/snd.h\
	../../include/carl/sndio.h

all: ../../include/carl ${FILES}

../../include/carl:; if test ! -d ../../include/carl ; then mkdir ../../include/carl ; fi

../../include/carl/sfheader.h: sfheader.h ; cp sfheader.h ../../include/carl
../../include/carl/crack.h: crack.h ; cp crack.h ../../include/carl
../../include/carl/argscan.h: argscan.h ; cp argscan.h ../../include/carl
../../include/carl/ircam.h: ircam.h ; cp ircam.h ../../include/carl
../../include/carl/snd.h: snd.h ; cp snd.h ../../include/carl
../../include/carl/sndio.h: sndio.h ; cp sndio.h ../../include/carl
../../include/carl/filesf.h: filesf.h ; cp filesf.h ../../include/carl
../../include/carl/defaults.h: defaults.h ; cp defaults.h ../../include/carl
../../include/carl/paths.h: paths.h ; cp paths.h ../../include/carl
../../include/carl/libsf.h: libsf.h ; cp libsf.h ../../include/carl

install:	../../include/carl ${FILES}
	date > install

clean:
	rm -f install
