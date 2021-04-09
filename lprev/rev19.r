/*
 * 
 * line format: <spec_char> <coeff> <length> 
 * 	spec_char :== t | c | a  (tap, comb, or allpass respectively)
 */

/* tap delay line parameters */
t  1.0  0.0
t  .841 0.0043
t  .504 0.0215
t  .491 0.0225
t  .379 0.0268
t  .380 0.0270
t  .346 0.0298
t  .289 0.0458
t  .272 0.0485
t  .192 0.0572
t  .193 0.0587
t  .217 0.0595
t  .181 0.0612
t  .180 0.0707
t  .181 0.0708
t  .176 0.0726
t  .142 0.0741
t  .167 0.0753
t  .134 0.0797
/* comb reverb parameters */
/* for ~~ 25KHz sampling rate */
/*c .46 .05 */
/*c .48 .056 */
/*c .50 .061 */
/*c .52 .068 */
/*c .53 .072 */
/*c .55 .078 */
/* for ~~ 50KHz sampling rate */
c .24 .05 
c .26 .056 
c .28 .061 
c .29 .068 
c .20 .072 
c .32 .078 
/* allpass reverb parameters */
a .7 .006  
a .683 0.0091 
a .721 .0099 
