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

	double f = 50;
	std::string instance_filename = "inst1.tsp";
	int u = std::numeric_limits<int>::max();

	if (argc >= 2 && argc%2==0) {
		instance_filename = argv[1];
	}
	else {
		throw "invalid parameter count, usage: program filename {-f Facilities} {-u Capacity}";
	}

	for (int i = 2; i < argc-1; i+=2) {
		std::string test = argv[i];
		if (std::string(argv[i]) == "-f") {
			f = std::stoi(argv[i+1]); //fix costs
		}
		if (std::string(argv[i]) == "-u") {
			u = std::stoi(argv[i+1]); //capacity
		}
	}
	
	Plane customers = readfile(instance_filename);

	Enumerator en(f, u);
	std::vector<Plane> partition;

	double tstart = clock();
	en.create_partition(partition, customers);
	double tstop = clock();
	//std::cout << "needed " << (tstop - tstart) / CLOCKS_PER_SEC << " seconds" << std::endl;
	en.print_result();
	return 0;
}