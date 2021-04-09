#
foreach n (*.c)
	sed -f sedscr $n > xxx
	if ($status != 0) exit(1)
	mv -f xxx $n
	if ($status != 0) exit(1)
	echo $n
end
