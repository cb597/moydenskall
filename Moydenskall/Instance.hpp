#ifndef INSTANCE_HPP
#define INSTANCE_HPP
#include <string>
#include "Partition.hpp"

class Instance {
public:
	Instance(int argc, char* argv[]);
	double f;
	std::string instance_filename;
	unsigned int u;
	unsigned int D;
	bool time_measurement;
	bool svg_ouput;
	Pointset customers;
private:
	Pointset readfile(std::string filename); // read a file in tsp format
};

#endif