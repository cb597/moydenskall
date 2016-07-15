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
	void run_kmeans(Partition&, unsigned int);
	void swamy(const Seeder&);
	Partition lloyds_algo(const Seeder& seeder, std::string filenamesuffix);
	Partition run_lloyd_all_k();
private:
	void kmeansstep(Partition&);
	Pointset customers;
	Instance instance;
};

#endif