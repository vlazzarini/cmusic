ins 0 dope;
seg b1 1 f2 d 0;	/* doppler shift */
mult b1 b1 p6;		/* multiplied against base freq */
osc b2 p5 p8 f3 d;
osc b1 b2 b1 f1 d;
out b1;
end;

var 0 s1 "loop.d";
gen 0 gen1 f3  0 0  .1 1  .6 .1 1 0;
gen 0 genraw f2 s1;
gen 0 gen2 f1 1 1;
note 0 dope 1 1 1000Hz p4sec .06sec;
ter;
