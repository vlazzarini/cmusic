# cmusic Makefile
# prepared by Victor Lazzarini, 2009

DESTDIR =	../bin
CFLAGS =	-O -I../include
LIBS =		../lib/libcarl.a -lm -lcurses 
OBJS =

all:		show cmpsig mshow

show:		show.o ${OBJS}
		${CC} show.o ${OBJS} ${LIBS} -o show

fshow:		fshow.o putx.o ${OBJS}
		${CC} fshow.o ${OBJS} putx.o ${LIBS} -o fshow

yshow:		yshow.o puty.o putx.o ${OBJS}
		${CC} yshow.o ${OBJS} puty.o putx.o ${LIBS} -o yshow

mshow:		mshow.o plotline.o ${OBJS}
		${CC} mshow.o plotline.o ${LIBS} -o mshow

cmpsig:		cmpsig.o plotline.o ${OBJS}
		${CC} cmpsig.o ${LIBS} -o cmpsig

install:	all
		cp show ${DESTDIR}
#		cp fshow ${DESTDIR}
		cp mshow ${DESTDIR}
		cp cmpsig ${DESTDIR}

clean:
		rm -f *.o show fshow yshow
