include(/usr/local/include/carl/config.m4)dnl

ALL =	ex1\
	ex10\
	ex11\
	ex12\
	ex13\
	ex14\
	ex15\
	ex16\
	ex17\
	ex18\
	ex19\
	ex2\
	ex20\
	ex21\
	ex22\
	ex23\
	ex24\
	ex25\
	ex3\
	ex4\
	ex5\
	ex6\
	ex7\
	ex8\
	ex9

all: ramp func_file nrout out

out:
	- for file in ${ALL} ; \
	do \
		if test ! -f ../exout/$${file}.out ; then \
			Player $${file}.P -o $${file} ; \
			if test $${file} = "ex16" ; then \
				$${file} 1 2 3 4 5 > ../exout/$${file}.out ; \
			elif test $${file} = "ex18" ; then \
				$${file} 2 > ../exout/$${file}.out ; \
			elif test $${file} = "ex24" ; then \
				$${file} < xyzzy > ../exout/$${file}.out ; \
			else \
				$${file} > ../exout/$${file}.out ; \
			fi ; \
		fi ; \
	done

nrout:
	- for file in ${ALL} ; \
	do \
		sh nrgrind $${file}.P > ../ex/$${file} ; \
	done

ramp:; gen1 -L24 0 0 1 1 > ramp

func_file:; gen3 -L101 0 1 100 100 > func_file

clean:; rm -f *.o *.c a.out core errs ${ALL} func_file ramp
