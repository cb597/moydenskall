#pragma once
#include <vector>
#include "Point.hpp"
#include "Tools.hpp"
#include "Seeder.h"


class KMeans {
public:
	KMeans(Pointset);
	void seed_static_and_run();
	void run(Pointset&, int);
	void swamy2();
	void swamyk(int k);
	void swamy(const Seeder&);
private:
	void kmeansstep(Pointset&, Pointset&);
	void assign(Pointset& sites);
	void assign_ball(Pointset& sites);
	Partition partition;
	int n; // number of customers
	Pointset customers;
};