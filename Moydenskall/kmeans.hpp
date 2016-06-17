#ifndef KMEANS_HPP
#define KMEANS_HPP
#include <vector>
#include "Point.hpp"
#include "Tools.hpp"
#include "Seeder.hpp"


class KMeans {
public:
	KMeans(Pointset);
	void seed_and_run(const Seeder&);
	void run(Pointset&, int);
	void swamy(const Seeder&);
private:
	void kmeansstep(Pointset&, Pointset&);
	Partition cluster(const Pointset& customers, Pointset& sites);
	void cluster_ball(Pointset& sites);
	Partition partition;
	int n; // number of customers
	Pointset customers;
};

#endif