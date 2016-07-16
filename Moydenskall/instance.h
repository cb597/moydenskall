#ifndef INSTANCE_HPP
#define INSTANCE_HPP
#include <string>
#include <vector>
#include "point.h"
using Pointset = std::vector<Point>;

class Instance {
public:
	Instance() {}
	Instance(int argc, char* argv[]);
	std::string instance_filename;
	Pointset customers;
	double fixed_costs() const { return fixed_costs_; }
	unsigned int capacity_limit() const { return capacity_limit_; }
	bool time_measurement() const { return time_measurement_; }
	bool svg_output() const { return svg_output_; }
	bool fixed_k() const { return fixed_k_; }
	double omega() const { return omega_; }
	unsigned int k() const { return k_; }
	void set_k(unsigned int k) { k_ = k; }
	unsigned int size() const { return D_; }
	double eps() const { return eps_; }
private:
	Pointset readfile(std::string filename); // read a file in tsp format
	double fixed_costs_;
	bool fixed_k_;
	unsigned int k_;
	unsigned int capacity_limit_;
	unsigned int D_;
	bool time_measurement_;
	bool svg_output_;
	double omega_;
	double eps_;
};

#endif