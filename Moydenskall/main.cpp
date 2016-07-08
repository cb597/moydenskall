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
#include <chrono>

int main(int argc, char* argv[]) {

	Instance instance = Instance(argc, argv);
	KMeans lloyd(instance);

	if (instance.time_measurement) {
		auto start = std::chrono::system_clock::now();

		if (instance.fixed_k)
			lloyd.lloyds_algo(ESeeder(instance, instance.k), instance.u, instance.f, "_singleRun_"+std::to_string(instance.k));
		else
			lloyd.run_lloyd_all_k();

		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "needed " << elapsed.count() << " milliseconds" << std::endl;
	}
	else {
		if (instance.fixed_k)
			lloyd.lloyds_algo(ESeeder(instance, instance.k), instance.u, instance.f, "_singleRun_" + std::to_string(instance.k));
		else
			lloyd.run_lloyd_all_k();
	}

	return 0;
}
