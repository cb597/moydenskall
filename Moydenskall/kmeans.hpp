#ifndef KMEANS_HPP
#define KMEANS_HPP
#include <vector>
#include "Point.hpp"
#include "Tools.hpp"
#include "Seeder.hpp"
#include "Partition.hpp"

class KMeans {
public:
	KMeans(Pointset&);
	void seed_and_run(const Seeder&);
	void run(Pointset&, int);
	void swamy(const Seeder&);
	void lloyds_algo(const Seeder& seeder, unsigned int capacity_limit);
private:
	void kmeansstep(Pointset&);
	Partition p;
	int k; // number of customers
	Pointset customers;
};

#endif