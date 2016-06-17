#include "kmeans.hpp"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>
#include "Seeder.hpp"





KMeans::KMeans(Pointset _customers) {
	customers = _customers;
	n = customers.size();
}

Partition KMeans::cluster(const Pointset& customers, Pointset& sites) {
	partition.clear();
	for (unsigned int i = 0; i < sites.size(); ++i) {
		partition.push_back(Pointset());
	}
	for (auto customer : customers) {
		double best_val = std::numeric_limits<double>::max();
		int best_id = -1;
		for (auto site : sites) {
			if (eucl2dist(site, customer) < best_val) {
				best_val = eucl2dist(site, customer);
				best_id = site.getId();
			}
		}
		if (best_id == -1)
			throw "couldn't cluster site in kmeansstep()";
		partition[best_id-1].push_back(customer);
	}
}

// cluster customers to sites if in ball
void KMeans::cluster_ball(Pointset& sites) {
	double rad = std::numeric_limits<double>::max();
	for (unsigned int i = 0; i < sites.size() - 1; ++i) {
		for (unsigned int j = i + 1; j < sites.size(); ++j) {
			rad = std::min(rad, eucl2dist(sites[i], sites[j]) / 9.);
		}
	}

	partition.clear();
	for (unsigned int i = 0; i < sites.size(); ++i) {
		partition.push_back(Pointset());
	}
	for (auto customer : customers) {
		for (auto site : sites) {
			if (eucl2dist(site, customer) < rad) {
				partition[site.getId() - 1].push_back(customer);
			}
		}
	}
}

// run a given seeding strategy and a single ball-k-means step
void KMeans::swamy(const Seeder& seeder) {
	Pointset sites = seeder.seed();
	// cluster customers within ball to one of the two sites
	cluster_ball(sites);
	print_to_svg(customers, partition, sites, seeder.toString() + "_init.svg");
	// move sites to centroid of points within ball
	sites = centroid(partition);
	print_to_svg(customers, partition, sites, seeder.toString() + "_result.svg");

	// no longer relevant for algorithm, but nice and complete graphical output:
	cluster(sites);
	print_to_svg(customers, partition, sites, seeder.toString() + "_final.svg");
}

void KMeans::kmeansstep(Pointset& customers, Pointset& sites) {
	cluster(sites);
	sites = centroid(partition);
}

void KMeans::seed_and_run(const Seeder& seeder) {
	Pointset sites = seeder.seed();
	cluster(sites);
	print_to_svg(customers, partition, sites, seeder.toString()+"init.svg");
	run(sites, 5);
	print_to_svg(customers, partition, sites, seeder.toString() + "result.svg");
}

void KMeans::run(Pointset& sites, int steps) {
	for (int i = 0; i < steps; ++i) {
		kmeansstep(customers, sites);
		print_to_svg(customers, partition, sites, std::to_string(i).append(".svg"));
	}
}