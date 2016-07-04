#ifndef EXTPART_HPP
#define EXTPART_HPP
#include "Tools.hpp"

class ExtPartition {
public:
	ExtPartition(const Pointset* _customers, const Pointset& _sites);
	ExtPartition(Pointset* _customers);
	ExtPartition();
	Partition getOldPartition(Pointset customers);
	unsigned int getMinTx();
	unsigned int assigned(unsigned int idx);
	unsigned int assigned_alternative(unsigned int idx);
	void delete_partition(unsigned int idx);
	Pointset centroids();
	Pointset ballkmeans(const Pointset& _sites);
	void createNewPartition(const Pointset& _sites);
	void print_to_svg(Pointset sites, std::string filename);
private:
	const Pointset* customers;
	double T;
	std::vector<double> Tx;
	unsigned int k;
	std::vector<unsigned int> id_1best;
	std::vector<double> val_1best;
	std::vector<unsigned int> id_2best;
	std::vector<double> val_2best;
	Partition createOldPartition(const Pointset& _customers);
};

#endif