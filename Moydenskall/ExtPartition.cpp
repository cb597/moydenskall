#include "ExtPartition.hpp"
#include <algorithm>
#include <vector>

ExtPartition::ExtPartition(const Pointset& _customers, const Pointset& _sites) {
	k = _sites.size();
	createNewPartition(_customers, _sites);
}

Partition ExtPartition::getOldPartition(Pointset customers) {
	return createOldPartition(customers);
}

unsigned int ExtPartition::getMinTx() {
	std::vector<double>::iterator result = std::min_element(std::begin(Tx), std::end(Tx));
	return std::distance(std::begin(Tx), result);
}

unsigned int ExtPartition::assigned(unsigned int idx) {
	return id_1best[idx];
}

unsigned int ExtPartition::assigned_alternative(unsigned int idx) {
	return id_2best[idx];
}

void ExtPartition::delete_partition(unsigned int idx) {
	for (unsigned int i = 0; i < id_1best.size(); ++i) {
		//reassign points assigned to the partition about to be deleted
		if (id_1best[i] == idx) {
			id_1best[i] = id_2best[i];
		}
		if (id_2best[i] == idx) {
			id_2best[i] = -1;
		}

		//adjust indices as a partition is missing now
		if (id_1best[i] > idx) --id_1best[i];
		if (id_2best[i] > idx) --id_2best[i];
	}
	--k;
}

Pointset ExtPartition::centroids(Pointset customers) {
	std::vector<Point> centers(k, Point(0,0));
	std::vector<unsigned int> counter(k, 0);
	for (unsigned int i = 0; i < customers.size(); ++i) {
		centers[assigned(i)].X += customers[i].X;
		centers[assigned(i)].Y += customers[i].Y;
		counter[assigned(i)]++;
	}
	for (unsigned int i = 0; i < k; ++i) {
		centers[i].X /= counter[i];
		centers[i].Y /= counter[i];
	}
	return centers;
}

//deals with legacy function calls, should be obsolete soon
Partition ExtPartition::createOldPartition(const Pointset& customers) {
	Partition partition = Partition();
	for (unsigned int i = 0; i < k; ++i) {
		partition.push_back(Pointset());
	}

	for (unsigned int i = 0; i < customers.size(); ++i) {
		partition[id_1best[i]].push_back(customers[i]);
	}
	return partition;
}

// calculate Voronoi regions and errors 
void ExtPartition::createNewPartition(const Pointset& customers, const Pointset& sites) {

	//init all vectors
	id_1best = std::vector<unsigned int>(customers.size(), 0);
	val_1best = std::vector<double>(customers.size(), std::numeric_limits<int>::max());
	id_2best = std::vector<unsigned int>(customers.size(), 0);
	val_2best = std::vector<double>(customers.size(), std::numeric_limits<int>::max());

	// loop over all customers and sites and determine best and secondbest site per customer
	for (unsigned int c = 0; c < customers.size(); ++c) {
		for (unsigned int s = 0; s < sites.size(); ++s) {
			double dist = eucl2dist(customers[c], sites[s]);
			if (dist < val_1best[c]) {
				id_2best[c] = id_1best[c];
				val_2best[c] = val_1best[c];
				id_1best[c] = s;
				val_1best[c] = dist;
			}
			else if (dist < val_2best[c]) {
				id_2best[c] = s;
				val_2best[c] = dist;
			}
		}
	}

	// calculate costs
	T = 0.; //cost of clustering customers around sites
	Tx = std::vector<double>(sites.size(), 0.); // cost of clustering customers around sites\{x}
	for (unsigned int i = 0; i < customers.size(); i++) {
		T += val_1best[i];
		for (unsigned int t = 0; t < sites.size(); ++t) {
			Tx[t] += t != id_1best[i] ? val_1best[i] : val_2best[i];
		}
	}

}
