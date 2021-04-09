include(/usr/local/include/carl/config.m4)dnl

FILES =	m4INCLUDE/sfheader.h\
	m4INCLUDE/crack.h\
	m4INCLUDE/libsf.h\
	m4INCLUDE/paths.h\
	m4INCLUDE/argscan.h\
	m4INCLUDE/defaults.h\
	m4INCLUDE/ircam.h\
	m4INCLUDE/filesf.h\
	m4INCLUDE/snd.h\
	m4INCLUDE/sndio.h

all: m4INCLUDE ${FILES}

m4INCLUDE:; if test ! -d m4INCLUDE ; then mkdir m4INCLUDE ; fi

m4INCLUDE/sfheader.h: sfheader.h ; cp sfheader.h m4INCLUDE
m4INCLUDE/crack.h: crack.h ; cp crack.h m4INCLUDE
m4INCLUDE/argscan.h: argscan.h ; cp argscan.h m4INCLUDE
m4INCLUDE/ircam.h: ircam.h ; cp ircam.h m4INCLUDE
m4INCLUDE/snd.h: snd.h ; cp snd.h m4INCLUDE
m4INCLUDE/sndio.h: sndio.h ; cp sndio.h m4INCLUDE
m4INCLUDE/filesf.h: filesf.h ; cp filesf.h m4INCLUDE
m4INCLUDE/defaults.h: defaults.h ; cp defaults.h m4INCLUDE
m4INCLUDE/paths.h: paths.h ; cp paths.h m4INCLUDE
m4INCLUDE/libsf.h: libsf.h ; cp libsf.h m4INCLUDE

install:	m4INCLUDE ${FILES}
	date > install

clean:
	rm -f install
