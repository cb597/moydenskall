#pragma once
#include <vector>
#include "Point.hpp"
#include "Tools.hpp"


class KMeans {
public:
	KMeans(Plane);
	Plane seed_static(int);
	void seed_static_and_run();
	Plane seed_random_subset(int n);
	void run(Plane&, int);
	void swamy2();
private:
	void kmeansstep(Plane&, Plane&);
	void assign(Plane& sites);
	void assign_ball(Plane& sites);
	Plane swamy2_sampling();
	double swamy2_probability_first(Point, Point);
	double swamy2_probability_second(Point, Point, Point);
	Partition partition;
	double d1; //square error of optimal solution for 1 site
	int n; // number of customers
	Plane customers;
	double drand();
};