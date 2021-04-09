# cmusic Makefile
# prepared by Victor Lazzarini, 2009

SFSYS =		bicsf

PROG =		cmusic
DESTDIR =	../bin
SRCDIR =	../../src
# SDY CFLAGS = 	-fsingle -O -D${SFSYS}
CFLAGS = 	-O -D${SFSYS} -I../include
LIBS =		-L../lib -lcarl -lbicsf -lm
MAIN = 		cmusic.o
SUBMAINS = 	mm.expr.o mm.field.o mm.gen.o mm.ins.o mm.merge.o \
		mm.play.o mm.print.o mm.printout.o mm.sec.o mm.set.o \
		mm.sndout.o mm.synth.o mm.ter.o mm.tersim.o mm.var.o
SUBS = 		m.askname.o m.getfield.o m.getp2.o m.getp4.o \
		m.gettime.o m.sfield.o m.trans.o
UGS = 		ug.abs.o ug.adn.o ug.airabsorb.o ug.band.o \
		ug.blp.o ug.delay.o ug.diff.o ug.div.o ug.expon.o \
		ug.flt.o ug.fltdelay.o ug.freq.o ug.illus.o \
		ug.integ.o ug.integer.o ug.inv.o ug.iosc.o \
		ug.janus.o \
		ug.logar.o ug.lookup.o ug.map.o ug.mult.o ug.neg.o ug.nres.o \
		ug.osc.o ug.out.o ug.quant.o ug.rah.o ug.ran.o \
		ug.sah.o ug.seg.o ug.shape.o ug.show.o \
		ug.showpath.o ug.signum.o ug.smooth.o ug.sndfile.o ug.space.o \
		ug.splice.o ug.sqroot.o ug.square.o ug.test.o \
		ug.trans.o ug.version.o ug.white.o  ug.zdelay.o
MMHEAD = 	m.defs.h m.structs.h m.exts.h
UGHEAD = 	ug.defs.h test.defs.h ug.exts.h ug.head.h
OBJS = 		$(SUBMAINS) $(SUBS) $(UGS)

${PROG}:	${PROG}.o ${OBJS}
		${CC} ${CFLAGS} ${PROG}.o ${OBJS} ${LIBS} -o ${PROG}

${PROG}.o: 	m.defs.h m.structs.h m.globs.h ug.defs.h test.defs.h ug.globs.h

mm.ins.o mm.synth.o  ug.test.o: $(MMHEAD) $(UGHEAD)

$(SUBMAINS) : 	$(MMHEAD)

$(UGS) : 	$(UGHEAD)

$(SUBS) :

install:	${PROG}
		cp ${PROG} ${DESTDIR}

clean:
		rm -f *.o core a.out ${PROG}


