Playertut: .test_examples print

print: nrefs.ig
	refer -a -sA -cL -P -p nrefs Player.tut | eqn -r576 -m2 | psroff -ms

test: nrefs.ig
	refer -a -sA -cL -P -p nrefs Player.tut | eqn -r200 -m2 | sunroff -ms

nrefs.ig:	nrefs
	indxbib nrefs

clean:
	rm -f nrefs.ig errs core a.out .test_examples
	cd exe; make clean

.test_examples:
	cd exe ; make
	date > .test_examples
