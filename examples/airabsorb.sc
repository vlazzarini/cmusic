#include <carl/cmusic.h>
set list = nres.list;

#define AIRABSORB(OUT,IN,X,Y) airabsorb OUT IN X Y d d d d d d d d d d
ins 0 test;
{carrier} 	osc  b1 p5 p6 f1 d;
		AIRABSORB(b1,b1,0,400);
		out b1;
end;

PULSE(f1);

note 0 test 1 0dB 1024Hz ;
ter;
