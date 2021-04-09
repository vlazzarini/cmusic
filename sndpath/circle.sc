set quad;

ins 0 quadrille;
seg b1 1 f3 d 0;	/* doppler shift */
mult b1 b1 p6;		/* multiplied against base freq */
osc b2 p5 p8 f2 d;	/* amp. envelope */
osc b1 b2 b1 f1 d;	/* carrier */

seg b2 1 f4 d 0;	/* global amp */
mult b1 b2 b1;		/* in b1 */

seg b2 1 f5 d 0;	/* chan 0 */
mult b3 b2 b1;		/* in b3 */
seg b2 1 f6 d 0;	/* chan 1 */
mult b4 b2 b1;		/* in b4 */
seg b2 1 f7 d 0;	/* chan 2 */
mult b5 b2 b1;		/* in b5 */
seg b2 1 f8 d 0;	/* chan 3 */
mult b6 b2 b1;		/* in b6 */

out b3 b4 b5 b6;
end;

gen 0 gen2 f1 1 1;			/* sine wave */
gen 0 gen1 f2  0 0  .1 1  .6 .1 1 0;	/* amp envelope for note */
var 0 s1 "circle.d";			/* doppler function name */
gen 0 genraw f3 s1;			/* loads doppler function */
var 0 s1 "-a circle.s";			/* global amp. function */
gen 0 quad f4 s1;
gen 0 gen0 f4;				/* optional normalization */
var 0 s1 "-1 circle.s";			/* channel 1 */
gen 0 quad f5 s1;
var 0 s1 "-2 circle.s";			/* channel 2 */
gen 0 quad f6 s1;
var 0 s1 "-3 circle.s";			/* channel 3 */
gen 0 quad f7 s1;
var 0 s1 "-4 circle.s";			/* channel 4 */
gen 0 quad f8 s1;

note 0 quadrille 5 1 1000Hz p4sec .06sec;
ter;
