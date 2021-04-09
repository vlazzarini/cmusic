/*
 * summation - return n'th element of summation series with incipits a and b
 * For the case of a = b = 1, the Fibonacci series is generated.
 * Note: series indexing is such that the b term is element 0.
 * Example: n = 4, a = 1, b = 1, ==> 5.
 */

long
summation(n, a, b)
	long n, a, b;
{
	if (n == 0)
		return(a);
	else if (n == 1)
		return(b);
	else
		return(summation(n - 1, a, b) + summation(n - 2, a, b));
}
