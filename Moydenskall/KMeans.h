#pragma once
#include <vector>
#include "Point.hpp"
#include "Tools.hpp"
#include "Seeder.h"


class KMeans {
public:
	KMeans(Pointset);
	void seed_and_run(const Seeder&);
	void run(Pointset&, int);
	void swamy(const Seeder&);
private:
	void kmeansstep(Pointset&, Pointset&);
	void assign(Pointset& sites);
	void assign_ball(Pointset& sites);
	Partition partition;
	int n; // number of customers
	Pointset customers;
};