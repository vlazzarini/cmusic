# cmusic Makefile
# prepared by Victor Lazzarini, 2009
# makefile for bicsf

C = ai

LIBS =		libbicsf
INCL =		include
ALIASES =	aliases
DOC =		doc
ORDER =  	../lib/libbicsf.a 
PROG = 	tosf

all:	${ORDER} ${PROG}

../lib/libbicsf.a: 
	for file in ${LIBS} ; \
	do \
		cd $${file} ; make ${MFLAGS}; cd .. ; \
	done
	date > .libraries

${PROG}: ../lib/libbicsf.a
	for file in ${PROG} ; \
	do \
		cd $${file} ; make ${MFLAGS}; cd .. ; \
	done
clean:
	for file in ${LIBS} ${PROG}; \
	do \
		cd $${file} ; make ${MFLAGS} clean ; cd ..;  \
	done
	rm ../lib/libbicsf.a
install:
	for file in ${PROG} ; \
	do \
		cd $${file} ; make ${MFLAGS} install; cd .. ; \
	done