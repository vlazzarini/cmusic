# cmusic Makefile
# prepared by Victor Lazzarini, 2009


PROG		= libbicsf.a
INCLUDE 	= -I../include -I../../include

LIBDIR =	../../lib
CFLAGS =	-O ${INCLUDE} -DMTU

OBJS =  	argscan.o\
		crack.o \
		disklock.o\
		floor.o \
		lmaxmin.o \
		malerr.o \
		notify.o \
		polish.o \
		sfexpr.o \
		strsav.o \
		fsndi.o \
		opensf.o \
		getflbuf.o \
		getsfname.o \
		getmaxamp.o \
		printsf.o \
		strsave.o \
		strstrip.o \
		setenv.o \
		sfcodes.o \
		tilde.o\
		yes.o

#		stripsuf.o 
#		setfilts.o

.c.o:
		${CC} ${CFLAGS} -c $*.c
		-ld  -r $*.o
		mv a.out $*.o

all:		${PROG} install

${PROG}: 	${OBJS} 
		rm -f ${PROG}
		ar q ${PROG} ${OBJS}
		ranlib ${PROG}

install:	${PROG}
		install ${PROG} ${LIBDIR}/${PROG}
		ranlib ${LIBDIR}/${PROG}
		date > install

clean:
		rm -f ${PROG} ${OBJS} core a.out install

${OBJS}:	../../include/carl/sndio.h \
		../../include/carl/filesf.h \
		../../include/carl/defaults.h \
		../../include/carl/paths.h \

