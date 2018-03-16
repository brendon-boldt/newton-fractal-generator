#include <complex>
#include <cmath>

using namespace std;

typedef std::complex<double> C;

const C a = C(2.0,-1.0);
const C b = C(1.0,-1.0);

C f(const C & z) {
	return pow(z, a) - 1.;
}
C fprime(const C & z) {
	return a * pow(z, b);
}
/* Weird
C f(const C & z) {
	return pow(z, 3.1) - 1.0;
}
C fprime(const C & z) {
	return 3.1 * pow(z, 2.1);
}
 */
/* Classic
C f(const C & z) { return pow(z, 3) - 1.0; }
C fprime(const C & z) { return 3.0 * pow(z, 2); }
 */

/* Good complex one
C f(const C & z) 	  { return pow(z, 6) + pow(z, 4) + 2.0 * pow(z, 1) - 1.0 ; }
C fprime(const C & z) { return 6.0 * pow(z, 5) + 4.0 * pow(z, 3) + 2.0 ; }
 */
