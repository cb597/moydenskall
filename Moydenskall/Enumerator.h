#pragma once
#include <vector>
#include "Point.h"
typedef std::vector<Point> Plane;
class Enumerator {
public:
	Enumerator(double f);
	void create_partition(std::vector<Plane>&, Plane&, double);
	void print_result();
	void svg_output();

private:
	Plane plane;
	double best_costs;
	Plane best_sites;
	std::vector<Plane> best_partition;
};