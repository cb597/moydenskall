#include "ExtPartition.hpp"

ExtPartition::ExtPartition(const Pointset& _customers, const Pointset& _sites) {
	createNewPartition(_customers, _sites);
	oldOne = createOldPartition(_customers, _sites);
}

Partition ExtPartition::getOldPartition() {
	return oldOne;
}

Partition ExtPartition::createOldPartition(const Pointset& customers, const Pointset& sites) {
	Partition partition = Partition();
	for (unsigned int i = 0; i < sites.size(); ++i) {
		partition.push_back(Pointset());
	}
	for (auto customer : customers) {
		double best_val = std::numeric_limits<double>::max();
		int best_id = -1;
		for (unsigned int i = 0; i < sites.size(); ++i) {
			if (eucl2dist(sites[i], customer) < best_val) {
				best_val = eucl2dist(sites[i], customer);
				best_id = i;
			}
		}
		if (best_id == -1) {
			throw "could not cluster site in kmeansstep()";
		}
		partition[best_id].push_back(customer);
	}
	return partition;
}

void ExtPartition::createNewPartition(const Pointset& customers, const Pointset& sites) {
	id_1best = std::vector<unsigned int>(customers.size(), -1);
	val_1best = std::vector<double>(customers.size(), std::numeric_limits<int>::max());
	id_2best = std::vector<unsigned int>(customers.size(), -1);
	val_2best = std::vector<double>(customers.size(), std::numeric_limits<int>::max());
	for (unsigned int c = 0; c < customers.size(); ++c) {
		for (auto s : sites) {
			double dist = eucl2dist(customers[c], s);
			if (dist < val_1best[c]) {
				id_2best[c] = id_1best[c];
				val_2best[c] = val_1best[c];
				id_1best[c] = s.getId();
				val_1best[c] = dist;
			}
			else if (dist < val_2best[c]) {
				id_2best[c] = s.getId();
				val_2best[c] = dist;
			}
		}
	}
	T = 0.; //cost of clustering customers around sites
	Tx = std::vector<double>(sites.size(), 0.); // cost of clustering customers around sites\{x}
	for (unsigned int i = 0; i < customers.size(); i++) {
		T += val_1best[i];
		for (unsigned int t = 0; t < sites.size(); ++t) {
			Tx[t] += t != id_1best[i] - 1 ? val_1best[i] : val_2best[i];
		}
	}

}
