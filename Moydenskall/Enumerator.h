#pragma once
#include <vector>
#include "Point.h"
#include "Evaluator.h"
typedef std::vector<Point> Plane;
class Enumerator {
public:
	Enumerator();
	void create_partition_of_n(std::vector<int>&, int, int);
	void create_partition(std::vector<Plane>&, Plane&);

private:
	Plane plane;
	Evaluator eval;
	double best_costs;
	Plane best_sites;
};