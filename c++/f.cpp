#include <complex>
#include <cmath>

using namespace std;

typedef std::complex<double> C;


C f(const C & z) {
	return
		pow(z, 6)
		+ pow(z, 4)
		+ 2.0 * pow(z, 1)
		- 1.0
		;
}

C fprime(const C & z) {
	return
		6.0 * pow(z, 5)
		+ 4.0 * pow(z, 3)
		+ 2.0
		;
}
