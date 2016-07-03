#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "Point.hpp"
#include <vector>
#include "ExtPartition.hpp"
using Pointset = std::vector<Point>;
using Partition = std::vector<Pointset>;

Pointset readfile(std::string filename); // read a file in tsp format
Point centroid(const Pointset&); // get centroid of points
double eucl2dist(Pointset, Point); // get sum of euclidean square distances from a site to all customers
double eucl2dist(Point, Point); // get euclidean square distance from two points
double evaluate_partition(Partition, Pointset, double); // get costs for a given partition and sites and fix_costs
std::vector<double> evaluate_partition(Partition, Pointset); // get costs foreach given partition and site individually
Partition cluster(const Pointset& customers, const Pointset& sites);
void print_to_svg(Pointset customers, ExtPartition partition, Pointset sites, std::string filename);
void print_to_svg(Pointset customers, Partition partition, Pointset sites, std::string filename);
double drand(); //random double in [0,1]

#endif