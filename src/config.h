#pragma once

#include <string>

struct CONFIG {
	long iters;
	double limit;
	long height;
	long width;
	double center_x;
	double center_y;
	double scale;
	std::complex<double> step;
	double delta;
	double color_scale;
	double color_range;
	double color_power;
};

CONFIG readConfig(std::string fn);
