#include "kmeans.hpp"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>
#include "Seeder.hpp"

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
	for (int i = 0; i < customers.size(); permutation.push_back(i++));
	std::random_shuffle(permutation.begin(), permutation.end());
	Pointset sites;
	for (int i = 1; i <= n; ++i) {
		sites.push_back(customers[permutation[i]]);
		sites[sites.size()-1].setId(i);
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

Pointset SwamykSeeder::seed() const{
	Pointset sites = Swamy2Seeder(customers).seed();

	std::vector<double> probability(customers.size(), std::numeric_limits<int>::max());
	double probability_sum = 0.;
	// init probability
	for (int i = 0; i < customers.size(); ++i) {
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

		for (int i = 0; i < customers.size(); ++i) {
			sum += probability[i] / probability_sum;
			if (sum > rand) {
				break;
			}
			sites.back() = customers[i];
		}
		sites.back().setId(i + 1);
		// update probabilities
		for (int i = 0; i < customers.size(); ++i) {
			double d = eucl2dist(customers[i], sites.back());
			if (probability[i] > d) {
				probability_sum + d - probability[i];
				probability[i] = d;
			}
		}

	}
	return sites;
}
