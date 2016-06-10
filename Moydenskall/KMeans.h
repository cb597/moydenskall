#pragma once
#include <vector>
#include "Point.hpp"
#include "Tools.hpp"


class KMeans {
public:
	KMeans(Pointset);
	Pointset seed_static(int);
	void seed_static_and_run();
	Pointset seed_random_subset(int n);
	void run(Pointset&, int);
	void swamy2();
	void swamyk(int k);
private:
	void kmeansstep(Pointset&, Pointset&);
	void assign(Pointset& sites);
	void assign_ball(Pointset& sites);
	Pointset swamy2_sampling();
	Pointset swamyk_sampling(int k);
	double swamy2_probability_first(Point, Point);
	double swamy2_probability_second(Point, Point, Point);
	Partition partition;
	double d1; //square error of optimal solution for 1 site
	int n; // number of customers
	Pointset customers;
	double drand();
};