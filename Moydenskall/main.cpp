//#include "Filereader.cpp"
#include <vector>
#include "Enumerator.h"
#include "Point.h"
#include <time.h>
#include <iostream>
#include "Tools.h"
#include <string>
#include <limits>


int main(int argc, char* argv[]) {

	//default values for command line parameters
	double f = 50;
	std::string instance_filename = "inst1.tsp";
	int u = std::numeric_limits<int>::max();
	bool time_measurement = false;
	bool svg_ouput = false;

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

	Plane customers = readfile(instance_filename);

	Enumerator en(f, u);
	std::vector<Plane> partition;

	if (time_measurement) {
		double tstart = clock();
		en.create_partition(partition, customers);
		double tstop = clock();
		std::cout << "needed " << (tstop - tstart) / CLOCKS_PER_SEC << " seconds" << std::endl;
	}
	else {
		en.create_partition(partition, customers);
	}

	en.print_result(svg_ouput);
	return 0;
}