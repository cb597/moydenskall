#include "Tools.h"
#include <fstream>
#include <string>

Point centroid(const Plane& plane) {
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
		sum += eucl2dist(site, p);
	}
	return sum;
}

double eucl2dist(Point a, Point b) {
	double xdiff = a.X - b.X;
	double ydiff = a.Y - b.Y;
	return xdiff*xdiff + ydiff*ydiff;
}

double evaluate_partition(Partition partition, Plane sites, double fix_costs) {
	if (sites.size() != partition.size()) {
		throw "incompatible amount of sites and partitions";
	}

	double sum = sites.size() * fix_costs;
	for (int i = 0; i < sites.size(); ++i) {
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
