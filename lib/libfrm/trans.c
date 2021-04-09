/* TRANS(A, ALPHA, B, N, OUTPUT) makes a transition from A to B in N steps
   according to transition parameter ALPHA. It stores the resulting N values
   starting at location OUTPUT. ALPHA = 0 will yield a straight line;
   ALPHA < 0 will yield an exponential transition, ALPHA > 0 will yield
   a logarithmic transition, according to the formula:
      OUTPUT[i] = A + (B - A) * (1 - exp( i*alpha/(n-1) )) / (1 - exp(alpha))
   for 0 <= i < n .
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

trans(a, alpha, b, n, output) 
 float a, alpha, b, *output; int n;
{
    register int i;
    register float d, e;
    register float interval;
    register float delta = b - a;

    if(n > 1) interval = 1./(n - 1.);
     else {fprintf(stderr, "trans: transition length <= 1\n"); exit(-1);}

    if(alpha != 0.0){
	register float denom;
	denom = 1./(1. - exp( (double) alpha));
	for(i=0; i<n; i++) 
	*output++ = a + delta*(1. - exp( (double) i*alpha*interval))*denom;
    } else 
	for(i=0; i<n; i++) *output++ = a + delta*i*interval;
}
