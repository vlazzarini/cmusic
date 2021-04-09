# cmusic Makefile
# prepared by Victor Lazzarini, 2009

PROG =		help

# where help program will live
DESTDIR 	=	../bin
# name it will assume on target system
PROGNAME =	help
# where help files will reside on target system
HELPDIR =	../lib/help
# where help files originate on CARL system
AT_CARL =	../lib/help
# where distributed help files reside on CARL system
HELP_DIST = 	../help/helpfiles
# root directory of program sources
SRCDIR =	..

HELPDIRS =	cmusic compose csound filters graphics lib \
		siganal siggens sigproc

CLFLAGS =	-Wno-comment -O -DHELP_DIR=\"${HELPDIR}\" -Dhelppath=\"${DESTDIR}/${PROG}\"

${PROG}:	${PROG}.c ${OBJS}
		${CC} ${PROG}.c ${OBJS} ${CLFLAGS} ${LIBS} -o ${PROG}

install:	${PROG} install_helpfiles # fix_source_listings
		cp ${PROG} ${DESTDIR}/${PROGNAME}

install_helpfiles:	
	-if test ! -d ${HELPDIR} ; \
	then \
		mkdir ${HELPDIR} ; \
		cd helpfiles ; \
		tar cf - . | ( cd ${HELPDIR} ; tar xf - ) ; \
	fi

fix_source_listings:
	rm -f ${HELPDIR}/sources/*
	csh mkhelpsrc ${SRCDIR} ${HELPDIR}/sources

pub:
	-if test ! -d ${HELP_DIST} ; \
	then \
		mkdir ${HELP_DIST} ; \
	fi
	cd ${AT_CARL} ; cp beginner help programs find_sources ${HELP_DIST}
	for DIR in "${HELPDIRS}" ; \
	do \
		cd ${AT_CARL} ; \
		tar cf - $${DIR} | ( cd ${HELP_DIST} ; tar xf - ) ; \
	done

clean:
	rm -f *.o ${PROG}
