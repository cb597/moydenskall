#include "kmeans.h"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>
#include "seeder.h"
#include "partition.h"
#include "instance.h"
#include <tuple>
#include <math.h>

KMeans::KMeans(Instance& _instance){
	instance = _instance;
	customers = _instance.customers;
}

// run a given seeding strategy and a single ball-k-means step
void KMeans::swamy(const Seeder& seeder) {
	Pointset sites = seeder.seed();
	Partition p = Partition(&customers, sites);
	p.setSites(sites);
	if (instance.svg_output()) p.print_to_svg(seeder.toString() + "_init.svg");
	// cluster customers within ball to one of the two sites
	// and move sites to centroid of points within ball
	p.ballkmeans();
	if (instance.svg_output()) p.print_to_svg(seeder.toString() + "_result.svg");
}

// seed and run lloyds algo until capacity limit is repected
Partition KMeans::lloyds_algo(const Seeder& seeder, std::string filenamesuffix) {
	
	Partition p = Partition(&customers, seeder.seed());

	//seed
	kmeansstep(p);
	if (instance.svg_output()) p.print_to_svg("lloyd_" + seeder.toString() + "_init.svg");

	// loop while largest partition violates capacity constraint
	while (p.capacity_check_and_clone(instance.capacity_limit())) {
		kmeansstep(p);
	}

	//final output
	if (instance.svg_output()) p.print_to_svg("lloyd_" + seeder.toString() + "_final"+filenamesuffix+".svg");
	return p;
}

Partition KMeans::run_lloyd_all_k() {
	//Sample2Seeder swamy2(instance);
	//SampleKSeeder swamyk(instance, 5);
	//StaticSeeder stat5(instance, 5);
	//SubsetSeeder subset(instance, 5);
	//GreedyDelSeeder gredel(instance, 5);
	//LTSeeder lseed(instance, 10);
	//DSeeder dseed(instance, 10);
	//ESeeder eseed(instance, 5);

	unsigned int startk = (unsigned int)std::ceil(instance.size() / instance.capacity_limit());
	unsigned int lg = (unsigned int)std::log(instance.size());
	std::vector<Partition> results = std::vector<Partition>();
	for (unsigned int k = startk; k <= lg + startk; ++k) {
		ESeeder eseed = ESeeder(instance);
		results.push_back(lloyds_algo(eseed, std::to_string(k)));
	}

	//get best result
	unsigned int best_id = 0;
	double val = std::numeric_limits<double>::max();
	for (unsigned int i = 0; i < results.size(); ++i) {
		if (results[i].evaluation(instance.fixed_costs()) < val) {
			best_id = i;
		}
	}
	return results[best_id];
}

void KMeans::kmeansstep(Partition& p) {
	p.setSites(p.centroids());
}

void KMeans::seed_and_run(const Seeder& seeder) {
	Partition p = Partition(&customers, seeder.seed());
	if (instance.svg_output()) p.print_to_svg(seeder.toString()+"init.svg");
	run_kmeans(p, 5);
	if (instance.svg_output()) p.print_to_svg(seeder.toString() + "result.svg");
	p.print_to_console(instance);
}

void KMeans::run_kmeans(Partition& p, unsigned int steps) {
	for (unsigned int i = 0; i < steps; ++i) {
		kmeansstep(p);
		if (instance.svg_output()) p.print_to_svg(std::to_string(i).append(".svg"));
	}
}