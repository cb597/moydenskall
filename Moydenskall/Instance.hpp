#ifndef INSTANCE_HPP
#define INSTANCE_HPP
#include <string>
#include <vector>
#include "Point.hpp"
using Pointset = std::vector<Point>;

class Instance {
public:
	Instance() {}
	Instance(int argc, char* argv[]);
	double f;
	bool fixed_k;
	unsigned int k;
	std::string instance_filename;
	unsigned int u;
	unsigned int D;
	bool time_measurement;
	bool svg_output;
	Pointset customers;
	double omega;
	double eps;
private:
	Pointset readfile(std::string filename); // read a file in tsp format
};

#endif