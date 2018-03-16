#include "CImg-2.2.0/CImg.h"
#include <complex>
#include <cmath>
#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <mutex>

#include "f.h"

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

/*
inline C poly(const vector<C> & coeffs, const C & z) {
	C val = C(0.0,0.0);
	for (int i = 0; i < coeffs.size(); ++i)
		val += pow(coeffs[i],i); 
	return val;
}

vector<C> deriv(const vector<C> & coeffs) {
	vector<C> coeffsP(coeffs.size()-1);
	for (int i = 0; i < coeffs.size()-1; ++i) {
		coeffsP[i] = coeffs[i+1] * C(i+1,0.0);
	}
	return coeffsP;
}
*/
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

double getX(const CONFIG & c, int i) {
	return c.center_x + (1.0*c.width/c.height) * c.scale * ((double) i/c.width - 0.5);
}

double getY(const CONFIG & c, int j) {
	return c.center_y - c.scale * ((double) j/c.height - 0.5);
}


vector<C> findRoots(const CONFIG & c) {
	mutex mtx;

	const double xMin = getX(c, 0);
	const double xMax = getX(c, c.width);
	const double yMin = getY(c, c.height);
	const double yMax = getY(c, 0);
	const int num_points = c.height * c.width / 10;
	//cout << xMin << " " << xMax << " " << yMin << " " << yMax << endl;
	vector<C> roots;


	#pragma omp parallel for
	for (int i = 0; i < num_points; i++) {
		C z = C(fRand(xMin, xMax), fRand(yMin, yMax));
		C fpz;


		for (int j = 0; j < c.iters; j++) {
			fpz = fprime(z);
			z = z - c.step * f(z)/fpz;
			//if (abs(z) > limit) break;
			if (abs(z) > c.limit) j = c.iters;
		}
		bool root_found = false;
		for (int r_i = 0; r_i < roots.size(); ++r_i) {
			if (abs(z - roots[r_i]) < c.delta) {
				root_found = true;
				break;
			}
		}
		if (!root_found) {
			mtx.lock();
			roots.push_back(z);
			mtx.unlock();
		}
	}
	// Side effect of of parallelizing the above loop is
	// getting repeat roots. Filter time!
	for (int i = 0; i < roots.size()-1; ++i) {
	    for (int j = i+1; j < roots.size(); ++j) {
		if (abs(roots[i] - roots[j]) < c.delta) {
		    roots.erase(roots.begin()+j);
		    --j;
		    continue;
		}
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

tuple<C, C, int, int> iterate(
		const CONFIG & c,
		const vector<C> & roots,
		C z) {
	C fpz, z_prev = z;
	double slope_delta = 1/c.limit;
	int i;
	for (i = 0; i < c.iters; ++i) {
		fpz = fprime(z);
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
 * click to zoom
*/

void plot(int type, CONFIG & c, string filename) {
	mutex mtx;
	above_limit = 0;
	no_root = 0;

	cimg_library::CImg<float> visu(c.width, c.height, 1, 3, 0);
	cimg_library::CImg<unsigned char> rgb_img(c.width, c.height, 1, 3, 0);
	vector< vector< tuple<C,C,int,int> > >
		vals(c.height, vector< tuple<C,C,int,int> >(c.width));

	double xMin = getX(c, 0);
	double xMax = getX(c, c.width);
	double yMin = getY(c, c.height);
	double yMax = getY(c, 0);
	vector<C> roots = findRoots(c);
	/*
	for (auto i : coeffs)
		cout << i << " ";
	cout << endl;
	for (auto i : coeffsP)
		cout << i << " ";
	cout << endl;
	return;
	 */
	for (auto i : roots)
		cout << i << " ";
	cout << endl;

	vector<unsigned int> iterCounter(c.iters, 0);

	#pragma omp parallel for
	for (int j = 0; j < c.height; ++j) {
		for (int i = 0; i < c.width; ++i) {
			//double z_x = c.center_x + (1.0*c.width/c.height) * c.scale * ((double) i/c.width - 0.5);
			double z_x = getX(c, i);
			//double x = c.center_x +
				//(xMax * (double)i/c.width + xMin * (1 - (double)i/c.width))/2;
			//double y = c.center_y +
				//(yMax * (double)j/c.height + yMin * (1 - (double)j/c.height))/2;
			double z_y = getY(c, j);
			vals[i][j] = iterate(c, roots, C(z_x,z_y));
			if (get<2>(vals[i][j]) >= 0) {
				mtx.lock();
				iterCounter[get<3>(vals[i][j])] += 1;
				mtx.unlock();
			}
			else {
				mtx.lock();
				iterCounter[c.iters-1] += 1;
				mtx.unlock();
			}
		}
	}

	unsigned int total = 0;
	for (int i = 0; i < c.iters; ++i) {
		total += iterCounter[i];
		iterCounter[i] = c.width * c.height - total;
	}

	#pragma omp parallel for
	for (int j = 0; j < c.height; ++j) {
		for (int i = 0; i < c.width; ++i) {
			C z = get<0>(vals[i][j]);
			C z_prev = get<1>(vals[i][j]);
			int r_i = get<2>(vals[i][j]);
			int iters = get<3>(vals[i][j]);

			double smooth = (log(c.delta) - log(abs(roots[r_i] - z_prev)))
				/ (log(abs(roots[r_i] - z)) - log(abs(roots[r_i] - z_prev)));
			smooth = min(1.0, max(smooth, 0.0));
			//double c_speed = 1.0 - c.color_scale*(log(iters+smooth)/log(c.iters+1));
			//double weighted = iterCounter[iters]*smooth + iterCounter[max(iters-1,0)]*(1-smooth);
			double weighted = iterCounter[iters]*(1-smooth) +
				iterCounter[min(iters+1,(int)iterCounter.size()-1)]*smooth;
			double iterSmooth = (weighted/2) / (c.height*c.width);
			double c_speed = 1.0 + c.color_range*(iterSmooth - 1);
			c_speed = min(1.0, max(0.0, c_speed));
			//c_speed = pow(c_speed, c.color_power);

			if (r_i >= 0) {
				//visu(i,j,min(r_i, 2)) =  c_speed * 255;
				visu(i,j,0) = 360.0 * r_i / roots.size();
				visu(i,j,1) = 1.0;
				visu(i,j,2) = c_speed;
			}
		}
	}


	rgb_img = visu.HSVtoRGB();
	rgb_img.blur(0.6);
	//rgb_img.blur_anisotropic(50.0);
	rgb_img.save_png(filename.c_str());

	if (type == 0) {
		//cout << "Points with no root:\t" << no_root << endl;
		//cout << "Points above limit:\t" << above_limit << endl;
		cimg_library::CImgDisplay main_disp(rgb_img, "test");
		unsigned int button;
		while (!main_disp.is_closed()) {
			main_disp.wait();
			button = main_disp.button();
			double new_center_x = getX(c, main_disp.mouse_x());
			double new_center_y = getY(c, main_disp.mouse_y());
			//cout << main_disp.mouse_x() << " " << main_disp.mouse_y() << endl;
			const double zoom = 2.0;
			if (button != 0) {
				c.center_x = new_center_x;
				c.center_y = new_center_y;
				cout << "New center: " << new_center_x
					<< ", " << new_center_y
					<< " x" << c.scale
					<< " d @ " << c.delta << endl;
				if (button == 1) {
					c.scale /= zoom;
					//c.limit /= zoom; 
					//c.delta /= zoom;
				} else if (button == 2) {
					c.scale *= zoom;
					//c.limit *= zoom; 
					//c.delta *= zoom;
				}
				main_disp.close();
				return plot(type, c, filename.c_str());
			}
		}
	}
}

int main() {
	// 0 - still image, 1 - animation
	int type;
	cin >> type;

	CONFIG c;
	cin >>
		c.iters >>
		c.limit >> c.delta >>
		c.width >> c.height >>
		c.scale >>
		c.center_x >> c.center_y >>
		c.color_range;
	double x,y;
	cin >> x >> y;
	c.step = C(x,y);

	switch (type) {
	case 0: {
		plot(type, c, "out.png");
		break;
	}
	case 1: {
		string prefix = string("ani/frame_");
		for (int i = 0; i < 60; i++) {
			cout << "Generating frame: " << i << endl;
			c.step = C(1.3, i * 1.0/60);
			plot(type, c, prefix + to_string(i));
		}
		break;
	}
	default:
		break;
	}

	return 0;
}
