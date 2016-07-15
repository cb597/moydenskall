#ifndef PART_HPP
#define PART_HPP
#include <tuple>
#include <vector>
#include "Point.hpp"
#include "Instance.hpp"
using Pointset = std::vector<Point>;

double eucl2dist(Point a, Point b);

class Partition {
public:
	Partition(const Pointset* _customers, const Pointset& _sites);
	Partition(Pointset* _customers) : customers(_customers) {};
	Partition() {};
	unsigned int getMinTx();
	unsigned int getMinWeightedTx();
	unsigned int assigned(unsigned int idx);
	unsigned int assigned_alternative(unsigned int idx);
	void delete_partition(unsigned int idx);
	Pointset centroids();
	void ballkmeans();
	void setSites(const Pointset& _sites);
	void print_to_svg(std::string filename);
	void print_to_console(const Instance& instance);
	Pointset centroid_estimation(Pointset & init_centers, double omega, double eps);
	double evaluation(double fixed_costs);
	Pointset getSites();
	bool capacity_check_and_clone(unsigned int limit);
private:
	const Pointset* customers;
	Pointset sites;
	Point centroid(Pointset & points) const;
	double TotalError;
	std::vector<double> Tx;
	unsigned int k;
	std::vector<unsigned int> id_1best;
	std::vector<double> val_1best;
	std::vector<unsigned int> id_2best;
	std::vector<double> val_2best;
	std::vector<unsigned int> partition_size;
	void subsetcentroids(Pointset & result, Pointset & set, Pointset & chosen, unsigned int position, unsigned int left) const;
	double get_optimal_candidates(std::vector<Pointset> & candidates, Pointset & chosen, unsigned int cur_part, double bestval, Pointset & bestset);
};

#endif