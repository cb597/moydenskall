#include "Instance.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <fstream>


Instance::Instance(int argc, char * argv[]) {

	//default values for command line parameters
	f = 50;
	instance_filename = "inst1.tsp";
	u = std::numeric_limits<unsigned int>::max();
	time_measurement = false;
	svg_ouput = false;

	if (argc >= 2 && argc % 2 == 0) {
		instance_filename = argv[1];
	}
	else {
		throw "invalid parameter count, usage: program filename {-f facilities} {-u capacity} {-t bool_time_measurement}";
	}

	// parse command line parameters
	for (int i = 2; i < argc - 1; i += 2) {
		if (std::string(argv[i]) == "-f") {
			f = std::stoi(argv[i + 1]); //fix costs
		}
		if (std::string(argv[i]) == "-u") {
			u = std::stoi(argv[i + 1]); //capacity
		}
		if (std::string(argv[i]) == "-time") {
			time_measurement = std::string(argv[i + 1]) == "true" ? true : false; //time measurement
		}
		if (std::string(argv[i]) == "-svg") {
			svg_ouput = std::string(argv[i + 1]) == "true" ? true : false; //svg_ouput
		}
	}
	customers = readfile(instance_filename);
	D = customers.size();
}

Pointset Instance::readfile(std::string filename) {

	std::ifstream file(filename);
	std::string line = "";
	Pointset input_points;

	while (line != "NODE_COORD_SECTION") {
		file >> line;
	}
	file >> line;

	double x, y;
	int counter = 1;
	for (std::string k = line; k != "EOF"; file >> k) {
		file >> x >> y;
		Point p(x, y, counter++);
		input_points.push_back(p);
	}

	return input_points;
}

