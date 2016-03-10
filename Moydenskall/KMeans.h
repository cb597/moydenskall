#pragma once
#include <vector>
#include "Point.h"
#include "Tools.h"


class KMeans {
public:
	KMeans();
	Plane seed_static(int);
	void seed_static_and_run(Plane);
	void run(Plane, Plane, int);
private:
	void kmeansstep(Plane&, Plane&);
};