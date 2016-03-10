//#include "Filereader.cpp"
#include <vector>
#include "Enumerator.h"
#include "Point.h"
#include <time.h>
#include <iostream>
#include "Tools.h"
#include <string>
#include "KMeans.h"

#define KMEANS
//#define ENTRYEXERCISE

int main(int argc, char* argv[]) {

	double f = 50;
	std::string instance_filename = "inst1.tsp";

	if (argc >= 2) {
		instance_filename = argv[1];
	}

	if (argc >= 3) {
		f = std::stoi(argv[2]);
	}


	
	Plane customers = readfile(instance_filename);
#ifdef KMEANS
	KMeans lloyd;
	lloyd.seed_static_and_run(customers);

#endif

#ifdef ENTRYEXERCISE
	Enumerator en(f);
	Partition partition;

	double tstart = clock();
	en.create_partition(partition, customers, f);
	double tstop = clock();
	//std::cout << "needed " << (tstop - tstart) / CLOCKS_PER_SEC << " seconds" << std::endl;
	en.print_result();
#endif

	return 0;
}