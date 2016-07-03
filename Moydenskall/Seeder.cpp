#include "kmeans.hpp"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>
#include "Seeder.hpp"
#include <tuple>
#include <numeric>

// Seeder Classes


// choose (0,0), (1,1),... as init sites
Pointset StaticSeeder::seed() const {
	Pointset sites;
	for (int i = 1; i <= n; ++i) {
		sites.push_back(Point(i, i, i));
	}
	return sites;
}

// select a random subset of the customers as init sites
Pointset SubsetSeeder::seed() const {
	std::srand(unsigned(std::time(NULL)));
	std::vector<int> permutation;
	for (unsigned int i = 0; i < customers.size(); permutation.push_back(i++));
	std::random_shuffle(permutation.begin(), permutation.end());
	Pointset sites;
	for (int i = 1; i <= n; ++i) {
		sites.push_back(customers[permutation[i]]);
		sites[sites.size() - 1].setId(i);
	}
	return sites;
}



Pointset Swamy2Seeder::seed() const {
	std::srand(unsigned(std::time(NULL)));
	Point cen = centroid(customers);
	double d1 = eucl2dist(customers, cen);

	double random1 = drand();
	double random2 = drand();

	double sum = 0.;
	Pointset sites = Pointset();
	for (auto c : customers) {
		sum += ((d1 + (double)customers.size()*eucl2dist(c, cen)) / (2.*(double)customers.size()*d1));
		if (sum > random1) {
			sites.push_back(c);
			sites[0].setId(1);
			break;
		}
	}
	sum = 0.;
	for (auto c : customers) {
		sum += (eucl2dist(sites[0], c) / (d1 + (double)customers.size()*eucl2dist(cen, sites[0])));
		if (sum > random2) {
			sites.push_back(c);
			sites[1].setId(2);
			break;
		}
	}

	return sites;
}

Pointset SwamykSeeder::seed() const {
	Pointset sites = Swamy2Seeder(customers).seed();

	std::vector<double> probability(customers.size(), std::numeric_limits<int>::max());
	double probability_sum = 0.;
	// init probability
	for (unsigned int i = 0; i < customers.size(); ++i) {
		for (auto s : sites) {
			probability[i] = std::min(probability[i], eucl2dist(customers[i], s));
		}
		probability_sum += probability[i];
	}

	// add k-2 sites
	for (int i = 2; i < k; ++i) {
		double sum = 0;
		double rand = drand();

		sites.push_back(customers[0]);

		for (unsigned int i = 0; i < customers.size(); ++i) {
			sum += probability[i] / probability_sum;
			if (sum > rand) {
				break;
			}
			sites.back() = customers[i];
		}
		sites.back().setId(i + 1);
		// update probabilities
		for (unsigned int i = 0; i < customers.size(); ++i) {
			double d = eucl2dist(customers[i], sites.back());
			if (probability[i] > d) {
				probability_sum += d - probability[i];
				probability[i] = d;
			}
		}

	}
	return sites;
}

Pointset GreedyDelSeeder::seed() const {
	return seed(customers);
}
Pointset GreedyDelSeeder::seed(Pointset init) const {

	Pointset sites = init;

	while (sites.size() > (unsigned int)k) {
		// B1
		std::vector<int> id_1best = std::vector<int>(customers.size(), -1);
		std::vector<double> val_1best = std::vector<double>(customers.size(), std::numeric_limits<int>::max());
		std::vector<int> id_2best = std::vector<int>(customers.size(), -1);
		std::vector<double> val_2best = std::vector<double>(customers.size(), std::numeric_limits<int>::max());
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
		double T = 0.;
		std::vector<double> Tx = std::vector<double>(sites.size(), 0.); // cost of clustering customers around sites\{x}
		for (unsigned int i = 0; i < customers.size(); i++) {
			T += val_1best[i];
			for (unsigned int t = 0; t < sites.size(); ++t) {
				Tx[t] += (int)t != id_1best[i] - 1 ? val_1best[i] : val_2best[i];
			}
		}

		// B2
		double bestval = std::numeric_limits<double>::max();
		int bestid = -1;
		for (unsigned int i = 0; i < sites.size(); ++i) {
			if (Tx[i] < bestval) {
				bestval = Tx[i];
				bestid = sites[i].getId();
			}
		}

		// B3
		Partition p = Partition(sites.size(), std::vector<Point>());
		for (unsigned int i = 0; i < customers.size(); ++i) {
			int topart = id_1best[i] != bestid ? id_1best[i] : id_2best[i];

			p[topart - 1].push_back(customers[i]);
		}
		if (p[bestid - 1].size()>0) {
			throw "greedy delete has gone terribly wrong delete has failed";
		}
		p.erase(p.begin() + bestid - 1);
		std::cout << "just erased " << bestid - 1 << "\n";
		sites = centroid(p);
	}

	return sites;
}

Pointset LTSeeder::seed() const {

	//C1
	double e = 0.0123;
	double p1 = sqrt(e);
	int N = (int)(2 * k / (1 - 5 * p1) + 2 * log(2 / p1) / pow((1 - 5 * p1), 2));

	SwamykSeeder swamykseeder(customers, N);
	auto S = swamykseeder.seed();

	//C2
	auto partition = cluster(customers, S);
	auto sdach = centroid(partition);


	GreedyDelSeeder greedydelseeder(customers, k);

	return greedydelseeder.seed(sdach);
}



Pointset DSeeder::ballkmeansstep(Pointset& sites) const {
	// cluster customers to sites if in ball
	double rad = std::numeric_limits<double>::max();
	for (unsigned int i = 0; i < sites.size() - 1; ++i) {
		for (unsigned int j = i + 1; j < sites.size(); ++j) {
			rad = std::min(rad, eucl2dist(sites[i], sites[j]) / 9.);
		}
	}

	Partition partition;
	for (unsigned int i = 0; i < sites.size(); ++i) {
		partition.push_back(Pointset());
	}
	for (auto customer : customers) {
		for (auto site : sites) {
			if (eucl2dist(site, customer) < rad) {
				partition[site.getId() - 1].push_back(customer);
			}
		}
	}
	return centroid(partition);
	//TODO this function has code duplicate with KMeans::cluster_ball()
}



Pointset DSeeder::seed() const {

	// D1 (obtain k initial centres using last seeding strategy)
	Pointset init = (LTSeeder(customers, k)).seed();

	// D2 (run a ball-k-means step)
	return ballkmeansstep(init);
}


void subsetcentroids(Pointset& result, Pointset& set, Pointset& chosen, unsigned int position, unsigned int left) {
	if (left == 0) {
		result.push_back(centroid(chosen));
		return;
	}
	else if (set.size()-position < left) {
		return;
	}
	else {
		chosen.push_back(set[position]);
		subsetcentroids(result, set, chosen, position + 1, left - 1);
		chosen.pop_back();
		subsetcentroids(result, set, chosen, position + 1, left);
	}
}

double get_optimal_candidates(const Pointset customers, Partition& p, Pointset& chosen, int cur_part, double bestval, Pointset& bestset) {
	if (cur_part == p.size()) {
		Partition part = cluster(customers, chosen);
		double val = evaluate_partition(part, chosen, 0);
		if (val < bestval) {
			bestval = val;
			bestset.clear();
			for (unsigned int i = 0; i < chosen.size(); ++i) {
				bestset.push_back(chosen[i]);
			}
		}
		return bestval;
	}
	for (unsigned int i = 0; i < p[cur_part].size(); ++i) {
		chosen.push_back(p[cur_part][i]);
		bestval = get_optimal_candidates(customers, p, chosen, cur_part + 1, bestval, bestset);
		chosen.pop_back();
	}
	return bestval;
}


Pointset ESeeder::seed() const {
	SwamykSeeder swamyk(customers, k);
	Pointset p = swamyk.seed();
	Partition part = cluster(customers, p);
	return centroid_estimation(part, p);
}

Pointset ESeeder::centroid_estimation(Partition& partition, Pointset & init_centers) const {
	Pointset centers;
	double eps = 0.5; // ToDo get real value
	double beta = 1 / (1 + 144 * eps*eps);
	double omega = 0.07; // Todo get real value

	Pointset assigned_center; //c_dach(x)
	Partition expanded_voronoi_regions;


	for (auto X : partition) {
		for (auto x : X) {
			Point minp = init_centers[0];
			double best = std::numeric_limits<double>::max();
			for (auto c : init_centers) {
				if(eucl2dist(c, x) < best) {
					minp = c;
					best = eucl2dist(c,x);
				}
			}
			assigned_center.push_back(minp);
		}
	}

	//set expanded voronoi region
	int pidx = 0;
	for (unsigned int i = 0; i < partition.size();++i) {
		Pointset voro = Pointset();
		auto X = partition[i];
		for (auto x : X) {
			for (auto c : init_centers) {
				if(eucl2dist(c, x) <= eucl2dist(x, assigned_center[pidx]) + eucl2dist(init_centers[i], assigned_center[pidx])/4.){
					voro.push_back(x);
				}
			}
			++pidx;
		}
		expanded_voronoi_regions.push_back(voro);
	}

	//random subset
	Partition random_subsets; // denoted as "S" in paper
	unsigned int amount = (int) (4 / (beta*omega));
	//as this is pretty weird we choose (ToDo)
	amount = 3;
	for(auto region : expanded_voronoi_regions) {
		Pointset subset = Pointset();
		std::vector<unsigned int> indices(region.size());
		std::iota(indices.begin(), indices.end(), 0);
		std::random_shuffle(indices.begin(), indices.end());
		for (unsigned int i = 0; i < amount; ++i) {
			if (i == region.size()) {
				break;
			}
			subset.push_back(region[indices[i]]);
		}
		random_subsets.push_back(subset);
	}

	// select centroids of all subsets of size 2/omega

	Partition candidates;
	for(auto s : random_subsets) {
		Pointset blubb = Pointset();
		unsigned int amount2 = (int)(2 / omega);
		//as this does not make sense we choose:
		amount2 = s.size() - 1;
		subsetcentroids(blubb, s, Pointset(), 0, amount2);
		candidates.push_back(blubb);
	}

	// select optimal candidate from each s \in subsetcentroid
	Pointset chosen = Pointset();
	Pointset bestset = Pointset();
	get_optimal_candidates(customers, candidates, chosen, 0, std::numeric_limits<double>::infinity() , bestset);


	return bestset;
}
