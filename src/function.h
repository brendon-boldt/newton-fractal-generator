#pragma once

typedef std::complex<double> C;

C f(const C & z);
C fprime(const C & z);

/*
inline C f(const C & z) {
	return
		pow(z, 6)
		+ pow(z, 4)
		+ 2.0 * pow(z, 3)
		- 1.0
		;
}

inline C fprime(const C & z) {
	return
		6.0 * pow(z, 5)
		+ 4.0 * pow(z, 3)
		+ 8.0 * pow(z, 2)
		;
}
 */

