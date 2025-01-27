#include "kmeans.h"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>
#include "seeder.h"
#include <tuple>
#include <numeric>
#include "partition.h"

// give a random double in [0,1] according to what current OS can do
double drand() {
	double d = 0.;
#ifdef _WIN32
	d = (double)rand() / (double)RAND_MAX;
#else
	d = drand48();
#endif
	return d;
}

// Seeder Classes


// choose (0,0), (1,1),... as init sites
Pointset StaticSeeder::seed() const {
	Pointset sites;
	for (int i = 1; i <= k; ++i) {
		sites.push_back(Point(i, i));
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
	for (int i = 1; i <= k; ++i) {
		sites.push_back(customers[permutation[i]]);
	}
	return sites;
}



Pointset Sample2Seeder::seed() const {
	std::srand(unsigned(std::time(NULL)));

	// get centroid of all customers...
	double x = 0, y = 0.;
	for (auto point : customers) {
		x += point.x();
		y += point.y();
	}
	x /= customers.size();
	y /= customers.size();
	Point cen = Point(x, y);
	// ...and its error
	double d1 = 0.0;
	for (Point p : customers) {
		d1 += eucl2dist(cen, p);
	}

	double random1 = drand();
	double random2 = drand();

	double sum = 0.;
	Pointset sites = Pointset();
	for (auto c : customers) {
		sum += ((d1 + (double)customers.size()*eucl2dist(c, cen)) / (2.*(double)customers.size()*d1));
		if (sum > random1) {
			sites.push_back(c);
			break;
		}
	}
	sum = 0.;
	for (auto c : customers) {
		sum += (eucl2dist(sites[0], c) / (d1 + (double)customers.size()*eucl2dist(cen, sites[0])));
		if (sum > random2) {
			sites.push_back(c);
			break;
		}
	}

	return sites;
}

Pointset SampleKSeeder::seed() const {
	Pointset sites = Sample2Seeder(instance).seed();

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

		for (unsigned int j = 0; j < customers.size(); ++j) {
			sum += probability[j] / probability_sum;
			if (sum > rand) {
				break;
			}
			sites.back() = customers[j];
		}
		// update probabilities
		for (unsigned int j = 0; j < customers.size(); ++j) {
			double d = eucl2dist(customers[j], sites.back());
			if (probability[j] > d) {
				probability_sum += d - probability[j];
				probability[j] = d;
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

		// B1 - get best and second best center for each customer
		Partition part = Partition(&customers, sites);

		// B2 - pick the center for which Tx is minimum
		int bestid = part.getMinTx();

		// B3 - delete chosen partition and move points to centroid of voronoi region
		part.delete_partition(bestid);
		sites = part.centroids();
	}

	return sites;
}

Pointset GreedyDelSeeder::seedWeighted(Pointset init) const {
	Pointset sites = init;

	while (sites.size() > (unsigned int)k) {

		// B1 - get best and second best center for each customer
		Partition part = Partition(&customers, sites);

		// B2 - pick the center for which Tx*|R(x)| is minimum
		int bestid = part.getMinWeightedTx();

		// B3 - delete chosen partition and move points to centroid of voronoi region
		part.delete_partition(bestid);
		sites = part.centroids();
	}

	return sites;
}

Pointset LTSeeder::seed() const {

	//C1
	double e = 0.0123; //ToDo get real value
	double p1 = sqrt(e);
	int N = (int)(2 * k / (1 - 5 * p1) + 2 * log(2 / p1) / pow((1 - 5 * p1), 2));

	SampleKSeeder swamykseeder(instance, N);
	auto S = swamykseeder.seed();

	//C2
	Partition partition = Partition(&customers, S);
	Pointset sdach = partition.centroids();


	GreedyDelSeeder greedydelseeder(instance);

	return greedydelseeder.seed(sdach);
}


Pointset DSeeder::seed() const {

	// D1 (obtain k initial centres using last seeding strategy)
	Pointset init = (LTSeeder(instance)).seed();
	Partition p = Partition(&customers, init);
	p.ballkmeans();
	// D2 (run a ball-k-means step)
	return p.getSites();
}


Pointset ESeeder::seed() const {
	SampleKSeeder swamyk(instance);
	Pointset p = swamyk.seed();
	Partition part = Partition(&customers, p);
	return part.centroid_estimation(p, instance.omega(), instance.eps());
}