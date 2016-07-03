#ifndef EXTPART_HPP
#define EXTPART_HPP
#include "Tools.hpp"

class ExtPartition {
public:
	Partition oldOne;
	ExtPartition(const Pointset& _customers, const Pointset& _sites);
	Partition getOldPartition();
	std::vector<unsigned int> id_1best;
	std::vector<double> val_1best;
	std::vector<unsigned int> id_2best;
	std::vector<double> val_2best;
	double T;
	std::vector<double> Tx;
private:
	Partition createOldPartition(const Pointset& _customers, const Pointset& _sites);
	void createNewPartition(const Pointset& _customers, const Pointset& _sites);
};

#endif