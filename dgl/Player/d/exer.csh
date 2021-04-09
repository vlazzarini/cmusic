#$
if ( ${#argv} < 1 ) then
	@ skip = 0
else
	set skipto = $argv[1]
	@ skip = 1
endif


foreach n (bug*.P v*.P p*.P spirlz.P x*.P)
if ($skip == 1) then
	if ($skipto != $n) then
		continue
	else
		@ skip = 0
	endif
endif
	
if (! -e ${n:r}) then
	Player -g $n -o ${n:r}
	if ($status != 0) exit 1
else
	echo $n
endif
echo ${n:r}
switch (${n:r})
		case v11:
			${n:r} 10 "gen1 -L100 0 60 1 120" "gen1 -L100 0 120 1 60" "4" > ${n:r}.out
			breaksw
		case p12:
		case p13:
			${n:r} < notes > ${n:r}.out
			breaksw
		case p26:
			${n:r} < score > ${n:r}.out
			breaksw
		case v06:
		case v07:
			${n:r} sinwave > ${n:r}.out
			breaksw;
		case v18:
			${n:r} one two three ${n:r}.out
			breaksw;
		case v20:
			${n:r} 1 30 > ${n:r}.out
			breaksw;
		case v26:
			v26 33 47 > ${n:r}.out
			breaksw;
		case v39:
			${n:r} 0 0 1 >> ${n:r}.out
			${n:r} 1 1 0 >> ${n:r}.out
			${n:r} 0 2 1 >> ${n:r}.out
			${n:r} 0 0 -1 >> ${n:r}.out
			${n:r} 0 2 -1 >> ${n:r}.out
			breaksw
		case v44:
			${n:r} 10 "gen1 -L32 0 60 1 120" "4" > ${n:r}.out
			breaksw
		case spirlz:
			${n:r} 0 1 5 10 > ${n:r}.out
			breaksw
		default:
			${n:r} > ${n:r}.out
			breaksw
endsw
if ($status != 0) exit(1)
end
exit 0
