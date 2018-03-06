#include <cstdlib>
#include "CImg-2.2.0/CImg.h"
#include <complex>
#include <cmath>
#include <tuple>
#include <vector>
#include <iostream>

//using namespace cimg_library;
using namespace std;

typedef std::complex<double> C;

struct CONFIG {
	int iters;
	double limit;
	int height;
	int width;
	std::complex<double> step;
	double delta;
	double color_scale;
	double color_range;
	double color_power;
};



/*
ITERS = 40
LIMIT = 1e10
SCALE = 10.0
SIZE = (200, 200)
STEP = 1.0 + 0j
DELTA = 1e0
COEFFS = [1, 0, 0, -1]
*/

C f(C z) {
	return pow(z, 3) - 1.0;
}

C fprime(C z) {
	return 3.0 * pow(z, 2);
}

vector<C> findRoots(vector<C> coeffs) {
	int step = 100;
	int iters = 1000;
	C newton_step = C(1.0, 0.0);
	double limit = 1e8;
	vector<C> roots;
	for (int i = 0; i < step; i++) {
		C fpz;
		C z = exp(C(0.0, i * 2 * M_PI / step));
		for (int j = 0; j < iters; j++) {
			fpz = fprime(z);
			//if (abs(fpz) < slope_delta)
			z = z - newton_step * f(z)/fpz;
			if (abs(z) > limit) break;
		}
		if (abs(z) > limit) break;
		bool root_found = false;
		for (int r_i = 0; r_i < roots.size(); ++r_i) {
			if (abs(z - roots[r_i]) < 1/limit) {
				root_found = true;
				break;
			}
		}
		if (!root_found) {
			roots.push_back(z);
		}
	}
	return roots;
}

/*
	for i in range(ITERS):
		fpz = fprime(z)
		if fpz == 0.0:
			return z
		z = z - STEP * f(z)/fpz
		if np.abs(z) > LIMIT:
			return LIMIT
	return z
*/

int above_limit = 0, no_root = 0;

tuple<C, C, int, int> iterate(const CONFIG & c, const vector<C> & roots, C z) {
	C fpz, z_prev = z;
	double slope_delta = 1/c.limit;
	int i;
	for (i = 0; i < c.iters; ++i) {
		fpz = fprime(z);
		//if (abs(fpz) < slope_delta)
		if (abs(fpz) == 0)
			break;

		z_prev = z;
		z = z - c.step * f(z)/fpz;

		for (int r_i = 0; r_i < roots.size(); ++r_i) {
			if (abs(roots[r_i] - z) < c.delta)
				return make_tuple(z, z_prev, r_i, i);
		}
		if (abs(z) > c.limit) {
			above_limit++;
			break;
		}
	}
	no_root++;
	return make_tuple(z, z_prev, -1, i);
}

double SCALE = 2.0;

/*
 * TODO:
 * variable coloring
*/

void plot(const CONFIG & c) {
	cimg_library::CImg<unsigned char> visu(c.width, c.height, 1, 3, 0);
	vector< vector<C> > vals(c.height, vector<C>(c.width));

	vector<C> coeffs;
	coeffs.push_back(C(-1,0));
	coeffs.push_back(C(0,0));
	coeffs.push_back(C(0,0));
	coeffs.push_back(C(3,0));
	vector<C> roots = findRoots(coeffs);
	for (auto i : roots)
		cout << i << " ";
	cout << endl;


	#pragma omp parallel for
	for (int j = 0; j < c.height; ++j) {
		for (int i = 0; i < c.width; ++i) {
			//z = SCALE * ((i/SIZE[0] - 0.5) + (0.5 - j/SIZE[1])*1.0j)
			double x = SCALE * ((double) i/c.width - 0.5);
			double y = -SCALE * ((double) j/c.height - 0.5);
			tuple<C, C, int, int> res = iterate(c, roots, C(x,y));
			//C z = res.first;
			C z = get<0>(res);
			C z_prev = get<1>(res);
			//int iters = res.second;
			int r_i = get<2>(res);
			int iters = get<3>(res);

			double smooth = (log(c.delta) - log(abs(roots[r_i] - z_prev)))
				/ (log(abs(roots[r_i] - z)) - log(abs(roots[r_i] - z_prev)));
			smooth = max(smooth, 1e-6);
			double c_speed = 1.0 - c.color_range 
				* c.color_scale*((double) log(iters+smooth)/log(c.iters));
			c_speed = min(1.0, max(0.0, c_speed));
			c_speed = pow(c_speed, c.color_power);

			if (r_i >= 0) {
				visu(i,j,min(r_i, 2)) =  c_speed * 255;
				//visu(i,j,min(r_i, 2)) = 255;
			}
		}
	}

	cout << "Points with no root:\t" << no_root << endl;
	cout << "Points above limit:\t" << above_limit << endl;

	cimg_library::CImgDisplay main_disp(visu, "test");

	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
}


int main() {
	CONFIG c;
	cin >>
		c.iters >>
		c.limit >> c.delta >>
		c.height >> c.width >>
		c.color_scale >> c.color_range >> c.color_power;
	double x,y;
	cin >> x >> y;
	c.step = C(x,y);
	/*
	c.iters = 20;
	c.limit = 1e3;
	c.height = 500;
	c.width = 500;
	c.delta = 1e-1;
	c.step = C(1.0, 1.0);
	 */

	plot(c);

	return 0;
}
