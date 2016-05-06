#include "KMeans.h"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>

KMeans::KMeans() {
}

void KMeans::assign(Plane& customers, Plane& sites) {
	partition.clear();
	for (int i = 0; i < sites.size(); ++i) {
		partition.push_back(Plane());
	}
	for (auto customer : customers) {
		double best_val = std::numeric_limits<double>::max();
		int best_id = -1;
		for (auto site : sites) {
			if (eucl2dist(site, customer) < best_val) {
				best_val = eucl2dist(site, customer);
				best_id = site.ID;
			}
		}
		if (best_id == -1)
			throw "couldn't assign site in kmeansstep()";
		partition[best_id - 1].push_back(customer);
	}
}

void KMeans::kmeansstep(Plane& customers, Plane& sites) {
	assign(customers, sites);
	sites = centroid(partition);
}

// choose (0,0), (1,1), (2,2) as init sites
Plane KMeans::seed_static(int n) {
	Plane centers;
	for (int i = 1; i <= n; ++i) {
		centers.push_back(Point(i, i, i));
	}
	return centers;
}

// select a random subset of the customers as init sites
Plane KMeans::seed_random_subset(Plane customers, int num_of_sites) {
	std::srand(unsigned(std::time(0)));
	std::random_shuffle(customers.begin(), customers.end());
	Plane centers;
	for (int i = 1; i <= num_of_sites; ++i) {
		customers[i].ID = i;
		centers.push_back(customers[i]);
	}
	return centers;
}

void KMeans::seed_static_and_run(Plane customers) {
	//Plane sites = seed_static(2);
	Plane sites = seed_random_subset(customers, 3);
	assign(customers, sites);
	print_to_svg(partition, sites, "init.svg");
	run(customers, sites, 5);
	print_to_svg(partition, sites, "result.svg");
}

void KMeans::run(Plane& customers, Plane& sites, int n) {
	for (int i = 0; i < n; ++i) {
		kmeansstep(customers, sites);
		print_to_svg(partition, sites, std::to_string(i).append(".svg"));
	}
}
