#ifndef KMEANS_HPP
#define KMEANS_HPP
#include <vector>
#include "point.h"
#include "seeder.h"
#include "partition.h"
#include "instance.h"

class KMeans {
public:
	KMeans(Instance&);
	void seed_and_run();
	void run_kmeans(Partition&, unsigned int);
	void swamy();
	Partition lloyds_algo(std::string filenamesuffix);
	Partition run_lloyd_all_k();
	void setSeeder();
private:
	void kmeansstep(Partition&);
	Pointset customers;
	Instance instance;
	Seeder seeder;
};

#endif