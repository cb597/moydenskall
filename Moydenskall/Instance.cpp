#include "Instance.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <stdexcept>


Instance::Instance(int argc, char * argv[]) {

	//default values for command line parameters
	f = 50;
	eps = 0.0005; //ToDo: get some useful value here
	omega = 4 / (3.1* 25 / (25 + 256 * eps*eps));
	instance_filename = "inst1.tsp";
	u = std::numeric_limits<unsigned int>::max();
	time_measurement = false;
	svg_output = false;

	if (argc >= 2 && argc % 2 == 0) {
		instance_filename = argv[1];
	}
	else {
		throw std::runtime_error("invalid parameter count, usage: program filename {-f facilities} {-u capacity} {-time bool_time_measurement} {-svg bool_svg_ouput}");
	}

	// parse command line parameters
	for (int i = 2; i < argc - 1; i += 2) {
		if (std::string(argv[i]) == "-f") {
			f = std::stoi(argv[i + 1]); //fix costs
		}
		else if (std::string(argv[i]) == "-u") {
			u = std::stoi(argv[i + 1]); //capacity
		}
		else if (std::string(argv[i]) == "-time") {
			time_measurement = std::string(argv[i + 1]) == "true" ? true : false; //time measurement
		}
		else if (std::string(argv[i]) == "-svg") {
			svg_output = std::string(argv[i + 1]) == "true" ? true : false; //svg_ouput
		}
		else if (std::string(argv[i]) == "-w") {
			omega = std::stod(argv[i + 1]); //omega value
		}
		else if (std::string(argv[i]) == "-k") {
			k = std::stoi(argv[i + 1]);
			fixed_k = true;
		}
		else {
			throw std::runtime_error("invalid parameter given, usage: program filename {-f facilities} {-u capacity} {-time bool_time_measurement} {-svg bool_svg_ouput}");
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
	for (std::string l = line; l != "EOF"; file >> l) {
		file >> x >> y;
		Point p(x, y);
		input_points.push_back(p);
	}

	return input_points;
}

