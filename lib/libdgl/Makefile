


LIBDIR =	../../lib
CFLAGS =	-O -I../../include

PROG =		libdgl
OBJS =		crack.o floatsav.o sfexpr.o funstuff.o summation.o \
		rd_func_fid.o rd_func_file.o

.c.o:
		${CC} ${CFLAGS} -c $*.c
		-ld -r $*.o
		mv a.out $*.o

${PROG}: 	${OBJS}
		ar r ../libcarl.a ${OBJS}

clean:
		rm -f ${OBJS}


