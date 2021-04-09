# cmusic Makefile
# prepared by Victor Lazzarini, 2009
# makefile for libraries

LIBDIR =	../lib

libs:		libcarl.a libieee.a

libcarl.a:	
		rm -f libcarl.a
		cd libfrm	; make ${MFLAGS}
		cd libjos	; make ${MFLAGS}
		cd libran	; make ${MFLAGS}		
		cd libdgl	; make ${MFLAGS}
		cd libasw	; make ${MFLAGS}
		cd libprocom	; make ${MFLAGS}
		cd libdgl	; make ${MFLAGS}

libieee.a:	
		cd libieee	; make ${MFLAGS}


install:	libcarl.a libieee.a
		ranlib ${LIBDIR}/libieee.a
		ranlib ${LIBDIR}/libcarl.a

clean:
		cd libfrm	; make ${MFLAGS} clean 
		cd libjos	; make ${MFLAGS} clean
		cd libieee	; make ${MFLAGS} clean
		cd libran	; make ${MFLAGS} clean
		cd libdgl	; make ${MFLAGS} clean
		cd libprocom	; make ${MFLAGS} clean
		cd libasw	; make ${MFLAGS} clean
		rm -f libcarl.a libieee.a
