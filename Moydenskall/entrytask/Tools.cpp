#include "Tools.hpp"
#include <fstream>
#include <string>

Point centroid(const Plane& plane) {

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

Plane centroid(const std::vector<Plane>& partition) {
	Plane centroids;
	int counter = 1;
	for (auto plane : partition) {
		centroids.push_back(centroid(plane));
		centroids.back().ID = counter++;
	}
	return centroids;
}

double eucl2dist(Plane plane, Point site) {
	double sum = 0.0;
	for (Point p : plane) {
		double xdiff = site.X - p.X;
		double ydiff = site.Y - p.Y;
		sum += xdiff*xdiff + ydiff*ydiff;
	}
	return sum;
}

double evaluate_partition(std::vector<Plane> partition, Plane sites, double fix_costs) {
	if (sites.size() != partition.size()) {
		throw "incompatible amount of sites and partitions";
	}

	double sum = sites.size() * fix_costs;
	for (unsigned int i = 0; i < sites.size(); ++i) {
		sum += eucl2dist(partition[i], sites[i]);
	}

	return sum;
}


Plane readfile(std::string filename) {
	
	std::ifstream file(filename);
	std::string line = "";
	Plane plane;

	while (line != "NODE_COORD_SECTION") {
		file >> line;
	}
	file >> line;

	double x, y;
	int counter = 1;
	for (std::string k = line; k != "EOF"; file >> k) {
		file >> x >> y;
		Point p(x, y, counter++);
		plane.push_back(p);
	}

	return plane;
}
