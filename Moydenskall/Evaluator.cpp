#include "Evaluator.h"

double Evaluator::evaluate(std::vector<Plane> partition, Plane sites) {
	if (sites.size() != partition.size()) {
		throw "incompatible amount of sites and partitions";
	}

	double sum = 0.0;
	for (int i = 0; i < sites.size(); ++i) {
		sum += eucl2dist(partition[i], sites[i]);
	}
	
	return sum;
}

double Evaluator::eucl2dist(Plane plane, Point site) {
	double sum = 0.0;
	for (Point p: plane) {
		double xdiff = site.X - p.X;
		double ydiff = site.Y - p.Y;
		sum += xdiff*xdiff + ydiff*ydiff;
	}
	return sum;
}
