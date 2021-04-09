#include <carl/cmusic.h>
set list = sp.list;
/* set stereo; */
set quad;
QUAD(10,100);
set funclength = 128K;
set blocklen = 512;
set revscale = .1;
set t60 = 3;
set cutoff = -60dB;
set srate = 44100;


ins 0 one;
{env}		seg  b4 p5 f4 d .1sec 0 .1sec;
{x}		osc  b2 p7 p8 f2 d;
{y}		osc  b3 p7 p9 f3 d;
{carrier} 	osc  b1 p5 p6 f1 d;
		mult b9 b2 .01;
{
		out b1, b1, b1, b1;
		out b3/1000, b3/1000, b3/1000, b3/1000;
		out b9, 0, 0, 0;
		out b9, 0, 0, 0;
}
		SPACE(b1,0) b2 b3 0 1 0dB;

end;

GEN3(f1) -20 20 -20 20;

{
GEN3(f2) -20 20 -20 20;
GEN3(f3) 20 20;
GEN3(f3) -50 50 -50 50;
}
GEN3(f3) -50 50 -50 50;
GEN3(f2) -50 50 -50 50;


GEN3(f4) 0 1 1 0;

note 0 one 8 0dB 240Hz 1 0.1Hz 0.1Hz;
{
note 0 one 8 0dB 240Hz 1 p4sec p4sec;
note 3 one 8 0dB 257Hz 10 4Hz 4Hz 11Hz 16;
note 6 one 8 0dB 57Hz 40 1Hz 1Hz 6Hz 0;
}

sec;
ter 10;
