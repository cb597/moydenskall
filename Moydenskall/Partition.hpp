#ifndef PART_HPP
#define PART_HPP
#include <tuple>
#include <vector>
#include "Point.hpp"
using Pointset = std::vector<Point>;

double eucl2dist(Point a, Point b);

class Partition {
public:
	Partition(const Pointset* _customers, const Pointset& _sites);
	Partition(Pointset* _customers);
	Partition();
	unsigned int getMinTx();
	unsigned int assigned(unsigned int idx);
	unsigned int assigned_alternative(unsigned int idx);
	void delete_partition(unsigned int idx);
	Pointset centroids();
	Pointset ballkmeans(const Pointset& _sites);
	void createNewPartition(const Pointset& _sites);
	void print_to_svg(const Pointset& sites, std::string filename);
	void print_to_console(const Pointset& sites);
	Pointset centroid_estimation(Pointset & init_centers);
	std::tuple<unsigned int, unsigned int> get_largest_partition();
private:
	const Pointset* customers;
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