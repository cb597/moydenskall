#include <vector>
#include "Point.hpp"
#include <time.h>
#include <iostream>
#include <string>
#include "Seeder.hpp"
#include "kmeans.hpp"
#include "Instance.hpp"
#include <limits>
#include <math.h>

int main(int argc, char* argv[]) {

	Instance instance = Instance(argc, argv);
	KMeans lloyd(instance);

	if (instance.time_measurement) {
		double tstart = clock();

		lloyd.run_lloyd_all_k();

		double tstop = clock();
		std::cout << "needed " << (tstop - tstart) / CLOCKS_PER_SEC << " seconds" << std::endl;
	}
	else {
		lloyd.run_lloyd_all_k();
	}

	return 0;
}
