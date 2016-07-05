#include "Tools.hpp"
#include <fstream>
#include <string>
#include <algorithm>
#include <tuple>
#include "ExtPartition.hpp"

Point centroid(const Pointset& plane) {
	if (plane.size() == 0) {
		return(Point(0, 0));
	}
	double x = 0.;
	double y = 0.;
	for (auto p : plane) {
		x += p.X;
		y += p.Y;
	}

	x /= plane.size();
	y /= plane.size();

	return Point(x,y);
}

Pointset centroid(const std::vector<Pointset>& partition) {
	Pointset centroids;
	int counter = 1;
	for (auto plane : partition) {
		centroids.push_back(centroid(plane));
		centroids.back().setId(counter++);
	}
	return centroids;
}

double eucl2dist(Pointset plane, Point site) {
	double sum = 0.0;
	for (Point p : plane) {
		sum += eucl2dist(site, p);
	}
	return sum;
}

double eucl2dist(Point a, Point b) {
	double xdiff = a.X - b.X;
	double ydiff = a.Y - b.Y;
	return xdiff*xdiff + ydiff*ydiff;
}

Pointset readfile(std::string filename) {
	
	std::ifstream file(filename);
	std::string line = "";
	Pointset input_points;

	while (line != "NODE_COORD_SECTION") {
		file >> line;
	}
	file >> line;

	double x, y;
	int counter = 1;
	for (std::string k = line; k != "EOF"; file >> k) {
		file >> x >> y;
		Point p(x, y, counter++);
		input_points.push_back(p);
	}

	return input_points;
}

double drand() {
	double d = 0.;
#ifdef _WIN32
	d = (double)rand() / (double)RAND_MAX;
#else
	d = drand48();
#endif
	return d;
}