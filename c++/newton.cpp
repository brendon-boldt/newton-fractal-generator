//#include <cstdlib>
#include "CImg-2.2.0/CImg.h"
#include <complex>
#include <cmath>
#include <tuple>
#include <vector>
#include <iostream>
#include <string>

//using namespace cimg_library;
using namespace std;

typedef std::complex<double> C;

struct CONFIG {
	int iters;
	double limit;
	int height;
	int width;
	double center_x;
	double center_y;
	double scale;
	std::complex<double> step;
	double delta;
	double color_scale;
	double color_range;
	double color_power;
};

C f(C z) {
	return pow(z, 3) - 1.0;
}

C fprime(C z) {
	return 3.0 * pow(z, 2);
}

vector<C> findRoots(vector<C> coeffs) {
	int num_points = 100;
	int iters = 1000;
	C newton_step = C(1.0, 0.0);
	double limit = 1e6;
	vector<C> roots;
	for (int i = 0; i < num_points; i++) {
		C fpz;
		C z = exp(C(0.0, i * 2 * M_PI / num_points));
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
			fpz = 1e-8;

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

/*
 * TODO:
 * variable coloring
 * click to zoom
 * extended root sampling
*/


void plot(const CONFIG & c, string filename) {
	above_limit = 0;
	no_root = 0;

	cimg_library::CImg<float> visu(c.width, c.height, 1, 3, 0);
	cimg_library::CImg<unsigned char> rgb_img(c.width, c.height, 1, 3, 0);
	vector< vector<C> > vals(c.height, vector<C>(c.width));

	vector<C> coeffs;
	coeffs.push_back(C(-1,0));
	coeffs.push_back(C(0,0));
	coeffs.push_back(C(0,0));
	coeffs.push_back(C(3,0));
	vector<C> roots = findRoots(coeffs);
	//for (auto i : roots)
		//cout << i << " ";
	//cout << endl;


	#pragma omp parallel for
	for (int j = 0; j < c.height; ++j) {
		for (int i = 0; i < c.width; ++i) {
			//z = SCALE * ((i/SIZE[0] - 0.5) + (0.5 - j/SIZE[1])*1.0j)
			double x = c.center_x + (1.0*c.width/c.height) * c.scale * ((double) i/c.width - 0.5);
			double y = -(c.center_y + c.scale * ((double) j/c.height - 0.5));
			tuple<C, C, int, int> res = iterate(c, roots, C(x,y));
			//C z = res.first;
			C z = get<0>(res);
			C z_prev = get<1>(res);
			//int iters = res.second;
			int r_i = get<2>(res);
			int iters = get<3>(res);

			double smooth = (log(c.delta) - log(abs(roots[r_i] - z_prev)))
				/ (log(abs(roots[r_i] - z)) - log(abs(roots[r_i] - z_prev)));
			smooth = min(1.0, max(smooth, 0.0));
			double c_speed = 1.0 - c.color_scale*(log(iters+smooth)/log(c.iters+1));
			c_speed = min(1.0, max(0.0, c_speed));
			c_speed = pow(c_speed, c.color_power);

			if (r_i >= 0) {
				//visu(i,j,min(r_i, 2)) =  c_speed * 255;
				visu(i,j,0) = 360.0 * r_i / roots.size();
				visu(i,j,1) = 1.0;
				visu(i,j,2) = c_speed;
			}
		}
	}

	//cout << "Points with no root:\t" << no_root << endl;
	//cout << "Points above limit:\t" << above_limit << endl;

	rgb_img = visu.HSVtoRGB();
	rgb_img.save_png(filename.c_str());

	/*
	cimg_library::CImgDisplay main_disp(rgb_img, "test");
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
	 */
}

void plot(const CONFIG & c) {
	plot(c, "out.png");
}



int main() {
	CONFIG c;
	cin >>
		c.iters >>
		c.limit >> c.delta >>
		c.width >> c.height >>
		c.scale >>
		c.center_x >> c.center_y >>
		c.color_scale >> c.color_range >> c.color_power;
	double x,y;
	cin >> x >> y;
	c.step = C(x,y);

	//plot(c);

	//return 0;
	string prefix = string("ani/frame_");
	for (int i = 0; i < 60; i++) {
		cout << "Generating frame: " << i << endl;
		c.step = C(1.3, i * 1.0/60);
		plot(c, prefix + to_string(i));
	}

	return 0;
}
