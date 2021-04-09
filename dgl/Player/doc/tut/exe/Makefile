# config.m4	1.4	(CMIL)	9/19/88	17:17:26

# flavor of UNIX in use
# type of CPU
# paths to libraries
# SDY define(m4CARLLIBPATH, m4LIBDIR/libcarl.a m4LIBDIR/libieee.a)dnl
# SDY define(m4CARLLIBS, m4LIBDIR/libcarl.a m4LIBDIR/libieee.a)dnl
# save for now SDY define(m4SFLIBS, m4LIBDIR/libsf.a)dnl
# Fortran libraries must be given in the following order for SUN3.x
# final resting place for installed software
# directory for include files
# where help files will live
# directory for tutorials 
# location of sources at CARL
# path to manual pages
# path to sys include directory
# path to vaxuba include directory
# dac/adc converters to use: one of ds (DSC200), ai (Digisound-16)
# sound filesystem to use: may be either csound or bicsf

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
