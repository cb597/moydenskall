//#include "Filereader.cpp"
#include <vector>
#include "Enumerator.h"
#include "Point.h"
#include <time.h>
#include <iostream>
#include "Tools.h"
#include <string>
#include "KMeans.h"
#include <limits>

#define KMEANS
//#define ENTRYEXERCISE

int main(int argc, char* argv[]) {

	double f = 50;
	std::string instance_filename = "inst1.tsp";
	int u = std::numeric_limits<int>::max();

	if (argc >= 2) {
		instance_filename = argv[1];
	}
	else {
		throw "filename missing, usage: program filename {-f Facilities} {-u Capacity}";
	}
	for (int i = 2; i < argc-1; i+=2) {
		std::string test = argv[i];
		if (std::string(argv[i]) == "-f") {
			f = std::stoi(argv[i+1]);
		}
		if (std::string(argv[i]) == "-u") {
			u = std::stoi(argv[i+1]);
		}
	}
	
	Plane customers = readfile(instance_filename);
#ifdef KMEANS
	KMeans lloyd;
	lloyd.seed_static_and_run(customers);

#endif

#ifdef ENTRYEXERCISE
	Enumerator en(f, u);
	Partition partition;

	double tstart = clock();
	en.create_partition(partition, customers);
	double tstop = clock();
	//std::cout << "needed " << (tstop - tstart) / CLOCKS_PER_SEC << " seconds" << std::endl;
	en.print_result();
#endif

	return 0;
}