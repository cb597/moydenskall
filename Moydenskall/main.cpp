//#include "Filereader.cpp"
#include <vector>
#include "Enumerator.h"
#include "Point.h"
#include <time.h>
#include <iostream>
#include "Tools.h"
#include <string>


int main(int argc, char* argv[]) {

	double f = 50;
	std::string instance_filename = "inst1.tsp";

	if (argc >= 2) {
		instance_filename = argv[1];
	}

	if (argc >= 3) {
		f = std::stoi(argv[2]);
	}


	
	Plane p = readfile(instance_filename);

	Enumerator en(f);
	std::vector<Plane> init;

	double tstart = clock();
	en.create_partition(init, p, f);
	double tstop = clock();
	//std::cout << "needed " << (tstop - tstart) / CLOCKS_PER_SEC << " seconds" << std::endl;
	en.print_result();
	return 0;
}