# cmusic Makefile
# prepared by Victor Lazzarini, 2009


CFLAGS =	-g -I../../include

PROG =		libprocom

OBJS =		fgetfloat.o fputfloat.o \
			getheader.o putheader.o \
			printprop.o \
			putprop.o getprop.o \
			rmprop.o getplist.o \
			putplist.o cpioheader.o \
			finitbuf.o fflushfloat.o putfc.o mkprop.o \
			globals.o addprop.o stdheader.o \
			fgetfbuf.o fputfbuf.o \
			fgetshort.o fputshort.o \
			fgetsbuf.o fputsbuf.o \
			fgetsample.o \
			fgetlong.o fputlong.o

.c.o:
		${CC} ${CFLAGS} -c $*.c
		-ld -r $*.o
		mv a.out $*.o


${PROG}: 	${OBJS}
		ar r ../libcarl.a ${OBJS}


clean:
		rm -f ${OBJS}


