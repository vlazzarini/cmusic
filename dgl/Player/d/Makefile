test_suite: setup files 

setup:	notefile score ramp sinwave

notefile:	; echo "no notefile!"

score:		; echo "no score!"

ramp:		; gen1 -L100 0 0 40 1 99 0 > ramp
sinwave:	; gen5 -L100 1 1 0 > sinwave

files:		; csh exer.csh

clean:
	rm -f ramp sinwave a.out core *.c *.o *.out P[0-9]*.[co]
	sh rmexe.sh .
