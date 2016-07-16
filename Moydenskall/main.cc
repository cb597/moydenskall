#include <iostream>
#include <string>
#include "seeder.h"
#include "kmeans.h"
#include "instance.h"
#include <limits>
#include <math.h>
#include <chrono>

int main(int argc, char* argv[]) {

	Instance instance = Instance(argc, argv);
	KMeans lloyd(instance);

	if (instance.time_measurement()) {
		auto start = std::chrono::system_clock::now();
		Partition p;
		if (instance.fixed_k())
			p = lloyd.lloyds_algo(ESeeder(instance), "_singleRun_"+std::to_string(instance.k()));
		else
			p = lloyd.run_lloyd_all_k();

		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "needed " << elapsed.count() << " milliseconds" << std::endl;
		p.print_to_console(instance);
	}
	else {
		Partition p;
		if (instance.fixed_k())
			p = lloyd.lloyds_algo(ESeeder(instance), "_singleRun_" + std::to_string(instance.k()));
		else
			p = lloyd.run_lloyd_all_k();
		p.print_to_console(instance);
	}

	return 0;
}
