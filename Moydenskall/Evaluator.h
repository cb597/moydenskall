#pragma once
#include <vector>
#include "Point.h"
typedef std::vector<Point> Plane;

class Evaluator {
public:
	double evaluate(std::vector<Plane>, Plane);

private:
	double eucl2dist(Plane, Point);
};
