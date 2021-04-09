#include <carl/cmusic.h>
set list = dist.list;
set stereo;
set revscale = .1;
set t60 = 3;
set cutoff = -60dB;


ins 0 one;
{env}		seg  b1 p5 f2 d 0;
{carrier} 	osc  b1 b1 p6 f1 d;
		SPACE(b1,1) p7 p8 0 1 1;
end;

GEN5(f1) 1,1,0 2,1,0 4,1,0; NORM(f1);
GEN4(f2) 0,0 -5 .1,1 -2 1,0;

#define BA 5
#define I .5

note    0 one .5 0dB 1000Hz 0 BA;
note p2+I one .5 0dB 1000Hz 0 BA+1;
note p2+I one .5 0dB 1000Hz 0 BA+2;
note p2+I one .5 0dB 1000Hz 0 BA+4;
note p2+I one .5 0dB 1000Hz 0 BA+8;
note p2+I one .5 0dB 1000Hz 0 BA+16;
note p2+I one .5 0dB 1000Hz 0 BA+32;
note p2+I one .5 0dB 1000Hz 0 BA+16;
note p2+I one .5 0dB 1000Hz 0 BA+8;
note p2+I one .5 0dB 1000Hz 0 BA+4;
note p2+I one .5 0dB 1000Hz 0 BA+2;
note p2+I one .5 0dB 1000Hz 0 BA+1;
note p2+I one .5 0dB 1000Hz 0 BA;

sec;
ter 3;
