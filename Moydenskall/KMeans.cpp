#include "KMeans.h"
#include <limits>

KMeans::KMeans() {
}

void KMeans::kmeansstep(Plane& customers, Plane& sites) {
	Partition partition;
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
		partition[best_id-1].push_back(customer);
	}
	sites = centroid(partition);
}

Plane KMeans::seed_static(int n) {
	Plane centers;
	for (int i = 1; i <= n; ++i) {
		centers.push_back(Point(i, i, i));
	}
	return centers;
}

void KMeans::seed_static_and_run(Plane customers) {
	Plane sites = seed_static(2);
	run(customers, sites, 5);
}

void KMeans::run(Plane customers, Plane sites, int n) {
	for (int i = 0; i < n; ++i) {
		kmeansstep(customers, sites);
	}
}
