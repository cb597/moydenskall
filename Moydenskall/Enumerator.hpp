#pragma once
#include <vector>
#include "Point.hpp"
typedef std::vector<Point> Plane;
class Enumerator {
public:
	Enumerator(double, int);
	void create_partition(std::vector<Plane>&, Plane&);
	void print_result(bool);
	void svg_output();

private:
	Plane plane;
	double best_costs;
	Plane best_sites;
	std::vector<Plane> best_partition;
	double fix_costs;
	int capacity;
};