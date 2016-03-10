#pragma once
#include "Point.h"
#include <vector>
typedef std::vector<Point> Plane;
typedef std::vector<Plane> Partition;
//typedef std::pair<Partition, Plane> Solution;

Plane readfile(std::string filename); // read a file in tsp format
Point centroid(const Plane&); // get centroid of points
Plane centroid(const std::vector<Plane>&); // get all centroids for a given partition
double eucl2dist(Plane, Point); // get sum of euclidean square distances from a site to all customers
double eucl2dist(Point, Point); // get euclidean square distance from two points
double evaluate_partition(Partition, Plane, double); // get costs for a given partition and sites and fix_costs
