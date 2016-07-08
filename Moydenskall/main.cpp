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
	KMeans lloyd(instance.customers);

	for (unsigned int k = (unsigned int)std::ceil(instance.D / instance.u); k <= std::log(instance.D); ++k) {
		ESeeder eseed = ESeeder(instance, k);
		lloyd.lloyds_algo(eseed, instance.u, std::to_string(k));
	}

	Sample2Seeder swamy2(instance);
	SampleKSeeder swamyk(instance, 5);
	StaticSeeder stat5(instance, 5);
	SubsetSeeder subset(instance, 5);
	GreedyDelSeeder gredel(instance, 5);
	LTSeeder lseed(instance, 10);
	DSeeder dseed(instance, 10);
	ESeeder eseed(instance, 5);

	if (instance.time_measurement) {
		double tstart = clock();
		lloyd.swamy(swamyk);
		lloyd.seed_and_run(subset);
		double tstop = clock();
		std::cout << "needed " << (tstop - tstart) / CLOCKS_PER_SEC << " seconds" << std::endl;
	}
	else {
		//lloyd.swamy(swamyk);
		lloyd.lloyds_algo(eseed, 70, "test");
		//lloyd.seed_and_run(eseed);
	}

	return 0;
}
