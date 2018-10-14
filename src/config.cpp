#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <complex>

#include "config.h"

using namespace std;

/*
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
*/

CONFIG readConfig(string fn) {
	CONFIG c;
	double step_a = 1.0, step_b = 0.0;

	ifstream infile(fn.c_str());

	string t, k, v_str;
	string line;
	while (getline(infile, line)) {
		t = "";
		k = "";
		v_str = "";
		istringstream iss(line);
		
		iss >> t >> k >> v_str;

		if (t == "int" || t == "long") {
			long v = strtol(v_str.c_str(), NULL, 10);
			if 		(k == "iters") c.iters = v;
			else if (k == "width") c.width = v;
			else if (k == "height") c.height = v;
		} else if (t == "double" || t == "float") {
			double v = strtod(v_str.c_str(), NULL);
			if 		(k == "limit") c.limit = v;
			else if (k == "delta") c.delta = v;
			else if (k == "scale") c.scale = v;
			else if (k == "center_x") c.center_x = v;
			else if (k == "center_y") c.center_y = v;
			//else if (k == "color_scale") c.color_scale = v;
			else if (k == "color_range") c.color_range = v;
			//else if (k == "color_power") c.color_power = v;
			else if (k == "step_a") step_a = v; // Intermediate value
			else if (k == "step_b") step_b = v; // Intermediate value
		} else if (t != "" && t != "#") {
			cout << "Warning, unidentified line beginning with \"" <<
				t << "\"" << endl;
		}
	}
	c.step = complex<double>(step_a, step_b);

	return c;
}

/*
int main() {
	auto c = readConfig("default.cfg");

	return 0;
}
*/
