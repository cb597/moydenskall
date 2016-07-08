#ifndef KMEANS_HPP
#define KMEANS_HPP
#include <vector>
#include "Point.hpp"
#include "Seeder.hpp"
#include "Partition.hpp"
#include "Instance.hpp"

class KMeans {
public:
	KMeans(Instance&);
	void seed_and_run(const Seeder&);
	void run_kmeans(Pointset&, int);
	void swamy(const Seeder&);
	double lloyds_algo(const Seeder& seeder, unsigned int capacity_limit, double fixed_costs, std::string filenamesuffix);
	void run_lloyd_all_k();
private:
	void kmeansstep(Pointset&);
	Partition p;
	Pointset customers;
	Instance instance;
};

#endif