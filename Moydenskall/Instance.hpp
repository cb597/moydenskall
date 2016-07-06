#ifndef INSTANCE_HPP
#define INSTANCE_HPP
#include <string>
#include "Tools.hpp"

class Instance {
public:
	Instance(int argc, char* argv[]);

	//default values for command line parameters
	double f = 50;
	std::string instance_filename = "inst1.tsp";
	int u = std::numeric_limits<int>::infinity();
	bool time_measurement = false;
	bool svg_ouput = false;
	Pointset customers;
private:
	Pointset readfile(std::string filename); // read a file in tsp format
};

#endif