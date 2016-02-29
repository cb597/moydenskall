#pragma once
#include <vector>
#include "Point.h"
typedef std::vector<Point> Plane;

class Evaluator {
public:
	double evaluate(std::vector<Plane>, Plane);
	void set_fix_costs(double);

private:
	double eucl2dist(Plane, Point);
	double fix_costs;
};
