#include "kmeans.hpp"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>
#include "Seeder.hpp"
#include "Partition.hpp"





KMeans::KMeans(Pointset& _customers) {
	p = Partition(&_customers);
	customers = _customers;
	k = customers.size();
}

// run a given seeding strategy and a single ball-k-means step
void KMeans::swamy(const Seeder& seeder) {
	Pointset sites = seeder.seed();
	p.createNewPartition(sites);
	// cluster customers within ball to one of the two sites
	p.print_to_svg(sites, seeder.toString() + "_init.svg");
	// move sites to centroid of points within ball
	sites = p.centroids();
	p.print_to_svg(sites, seeder.toString() + "_result.svg");
}

void KMeans::kmeansstep(Pointset& customers, Pointset& sites) {
	p.createNewPartition(sites);
	sites = p.centroids();
}

void KMeans::seed_and_run(const Seeder& seeder) {
	Pointset sites = seeder.seed();
	p.createNewPartition(sites);
	p.print_to_svg(sites, seeder.toString()+"init.svg");
	run(sites, 5);
	p.print_to_svg(sites, seeder.toString() + "result.svg");
	p.print_to_console(sites);
}

void KMeans::run(Pointset& sites, int steps) {
	for (int i = 0; i < steps; ++i) {
		kmeansstep(customers, sites);
		p.print_to_svg(sites, std::to_string(i).append(".svg"));
	}
}