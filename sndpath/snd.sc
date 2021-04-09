#define SndFileName "wdrum"
#define SndFileSampDur (48K)*.1
#define NoteDur 10

set quad;
var 0 s1 "wdrum";	/* can not use s1 for anything else! */

ins 0 snd;
	seg b2 1.0 f1 d 0;			/* doppler shift */
	seg b3 1.0 f2 d 0;
	sndfile b1 b3 b2 s1 1 0 SndFileSampDur d d;
/*sndfile output[b] amplitude[bvpn] increment[bvpn] filename[s] channel[vpn] */
/* startframe[bvpn] endframe[bvpn] pos[bvpnd] ptr[d]*/

	seg b2 1 f3 d 0;	/* chan 1 */
	mult b3 b2 b1;		/* in b3 */
	seg b2 1 f4 d 0;	/* chan 2 */
	mult b4 b2 b1;		/* in b4 */
	seg b2 1 f5 d 0;	/* chan 3 */
	mult b5 b2 b1;		/* in b5 */
	seg b2 1 f6 d 0;	/* chan 4 */
	mult b6 b2 b1;		/* in b6 */

	out b3 b4 b5 b6;
end;

var 0 s2 "-d loop.s";			/* dopplar function lasts DUR secs */
/* quad -L32 -d loop.s 10 */
gen 0 quad f1 s2 NoteDur;
var 0 s2 "-a loop.s";			/* global amp. function */
gen 0 quad f2 s2;
gen 0 gen0 f2;				/* optional normalization to 1 */

var 0 s2 "-1 loop.s";			/* channel 1 */
gen 0 quad f3 s2;
var 0 s2 "-2 loop.s";			/* channel 2 */
gen 0 quad f4 s2;
var 0 s2 "-3 loop.s";			/* channel 3 */
gen 0 quad f5 s2;
var 0 s2 "-4 loop.s";			/* channel 4 */
gen 0 quad f6 s2;

note 0 snd NoteDur;
ter;
