#pragma once
#include "Point.hpp"
#include <vector>
typedef std::vector<Point> Pointset;
typedef std::vector<Pointset> Partition;
//typedef std::pair<Partition, Pointset> Solution;

Pointset readfile(std::string filename); // read a file in tsp format
Point centroid(const Pointset&); // get centroid of points
Pointset centroid(const std::vector<Pointset>&); // get all centroids for a given partition
double eucl2dist(Pointset, Point); // get sum of euclidean square distances from a site to all customers
double eucl2dist(Point, Point); // get euclidean square distance from two points
double evaluate_partition(Partition, Pointset, double); // get costs for a given partition and sites and fix_costs
void print_to_svg(Pointset customers, Partition partition, Pointset sites, std::string filename);
double drand(); //random double in [0,1]