#include <carl/cmusic.h>

set list ;
set stereo ;

set funclength = 8K;

ins 0 toot;
	osc  b1 p5 p6 f1 d;
	/*
	out b1, b1;
	*/
	SPACE(b1,1) -10 10 0 1 1 ;
end;

SINE(f1) ;

note 0 toot 1 1 440Hz ;

sec ;

ter 2 ; 
