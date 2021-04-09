# cmusic Makefile
# prepared by Victor Lazzarini, 2009

CFLAGS =	-O

PROG =		libjos
OBJS =		cat.o catm.o err.o getbody.o getfield.o getfil.o getflt.o \
		gethead.o geti.o getio.o geto.o getsn.o gettail.o indexs.o \
		inint.o itoc.o makarr.o newstr.o prsnam.o putflt.o rmvhead.o \
		rmvtail.o sesyes.o

.c.o:
		${CC} ${CFLAGS} -c $*.c
		-ld -r $*.o
		mv a.out $*.o

${PROG}: 	${OBJS}
		ar r ../libcarl.a ${OBJS}

clean:
		rm -f ${OBJS}


