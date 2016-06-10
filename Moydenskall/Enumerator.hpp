#ifndef Enumerator_hpp
#define Enumerator_hpp
#include <vector>
#include "Point.hpp"
typedef std::vector<Point> Pointset;
class Enumerator {
public:
	Enumerator(double, int);
	void create_partition(std::vector<Pointset>&, Pointset&);
	void print_result(bool);
	void svg_output();

private:
	Pointset plane;
	double best_costs;
	Pointset best_sites;
	std::vector<Pointset> best_partition;
	double fix_costs;
	int capacity;
};

#endif