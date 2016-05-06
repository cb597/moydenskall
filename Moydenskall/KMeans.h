#pragma once
#include <vector>
#include "Point.hpp"
#include "Tools.hpp"


class KMeans {
public:
	KMeans();
	Plane seed_static(int);
	void seed_static_and_run(Plane);
	Plane seed_random_subset(Plane, int n);
	void run(Plane&, Plane&, int);
private:
	void kmeansstep(Plane&, Plane&);
	Partition partition;
};