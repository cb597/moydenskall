#ifndef EXTPART_HPP
#define EXTPART_HPP
#include "Tools.hpp"

class ExtPartition {
public:
	ExtPartition(const Pointset& _customers, const Pointset& _sites);
	Partition getOldPartition(Pointset customers);
	unsigned int getMinTx();
	unsigned int assigned(unsigned int idx);
	unsigned int assigned_alternative(unsigned int idx);
	void delete_partition(unsigned int idx);
	Pointset centroids(Pointset customers);
	Pointset ballkmeans(const Pointset& _customers, const Pointset& _sites);
private:
	double T;
	std::vector<double> Tx;
	unsigned int k;
	std::vector<unsigned int> id_1best;
	std::vector<double> val_1best;
	std::vector<unsigned int> id_2best;
	std::vector<double> val_2best;
	Partition createOldPartition(const Pointset& _customers);
	void createNewPartition(const Pointset& _customers, const Pointset& _sites);
};

#endif