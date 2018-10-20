#include <complex>
#include <cmath>

using namespace std;

typedef std::complex<double> C;

const C a = C(2.0,-1.0);
const C b = C(1.0,-1.0);
const C i = C(0.0, 1.0);

/**
 * Please note:
 *  - Uncomment a premade function or write your own and recompile (make)
 *  - If you want complex constants, set like above (do not construct them
 *    inside of the function.
 *  - Use WolframAlpha if you don't like doning derivatives yourself.
 * 	- C++ you must specify 4.0 instead 4 for type reasons (except in pow)
 */

C f(const C & z) { return pow(z, 3) - 1.0; }
C fprime(const C & z) { return 3.0 * pow(z, 2); }

/* Classic Newton Fractal
C f(const C & z) { return pow(z, 3) - 1.0; }
C fprime(const C & z) { return 3.0 * pow(z, 2); }
*/

/* Contains attracting cycles (divergent, black areas)
C f(const C & z) 	  { return (z+i)*(z-i)*(z+2.3)*(z-2.3) ; }
C fprime(const C & z) { return 4.0*pow(z,3)-8.58*z; }
*/

/* Good complex one
C f(const C & z) 	  { return pow(z, 6) + pow(z, 4) + 2.0 * pow(z, 1) - 1.0 ; }
C fprime(const C & z) { return 6.0 * pow(z, 5) + 4.0 * pow(z, 3) + 2.0 ; }
 */

/* Interesting sine
C f(const C & z) {
	return pow(z, 2) + sin(10.*z);
}
C fprime(const C & z) {
	return 2.*z + 10.*cos(10.*z);
}
*/

/*
C f(const C & z) {
	return pow(z, 2) + sin(10.*z);
}
C fprime(const C & z) {
	return 2.*z + 10.*cos(10.*z);
}
*/

/* Weird non-integer power
C f(const C & z) {
	return pow(z, 3.1) - 1.0;
}
C fprime(const C & z) {
	return 3.1 * pow(z, 2.1);
}
 */

/* 7-th degree polynomial (roots of unity)
C f(const C & z) { return pow(z, 7) - 1.0; }
C fprime(const C & z) { return 7.0 * pow(z, 6); }
*/

/* 5-th degree polynomial (roots of unity)
C f(const C & z) { return pow(z, 5) + 1.0; }
C fprime(const C & z) { return 5.0 * pow(z, 4); }
 */

/* Homework
C f(const C & z) {
	return
		pow(z, 5) +
		pow(z, 4) * -2. +
		pow(z, 3) * 4. +
		pow(z, 2) * -1. +
		z * 3. +
		- 2.;
}
C fprime(const C & z) {
	return
		pow(z, 4)*5. +
		pow(z, 3)*4. * -2. +
		pow(z, 2)*3. * 4. +
		z * -2. +
		3.;
}
 */
