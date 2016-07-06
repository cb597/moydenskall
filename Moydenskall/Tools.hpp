#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "Point.hpp"
#include <vector>
using Pointset = std::vector<Point>;

Point centroid(const Pointset&); // get centroid of points
double eucl2dist(Pointset, Point); // get sum of euclidean square distances from a site to all customers
double eucl2dist(Point, Point); // get euclidean square distance from two points
double drand(); //random double in [0,1]

#endif