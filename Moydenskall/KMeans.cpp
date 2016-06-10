#include "KMeans.h"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>

KMeans::KMeans(Pointset _customers) {
	customers = _customers;
	n = customers.size();
}

void KMeans::assign(Pointset& sites) {
	partition.clear();
	for (int i = 0; i < sites.size(); ++i) {
		partition.push_back(Pointset());
	}
	for (auto customer : customers) {
		double best_val = std::numeric_limits<double>::max();
		int best_id = -1;
		for (auto site : sites) {
			if (eucl2dist(site, customer) < best_val) {
				best_val = eucl2dist(site, customer);
				best_id = site.ID;
			}
		}
		if (best_id == -1)
			throw "couldn't assign site in kmeansstep()";
		partition[best_id-1].push_back(customer);
	}
}

// assign customers to sites if in ball
void KMeans::assign_ball(Pointset& sites) {
	double rad = std::numeric_limits<double>::max();
	for (int i = 0; i < sites.size() - 1; ++i) {
		for (int j = i + 1; j < sites.size(); ++j) {
			rad = std::min(rad, eucl2dist(sites[i], sites[j]) / 9.);
		}
	}

	partition.clear();
	for (int i = 0; i < sites.size(); ++i) {
		partition.push_back(Pointset());
	}
	for (auto customer : customers) {
		for (auto site : sites) {
			if (eucl2dist(site, customer) < rad) {
				partition[site.ID - 1].push_back(customer);
			}
		}
	}
}

// run seeding strategy for two sites and a single ball-2-means step
void KMeans::swamy2() {
	// sampling 2 initial sites according to algo
	Pointset sites = swamy2_sampling();
	// assign customers within ball to one of the two sites
	assign_ball(sites);
	print_to_svg(partition, sites, "swamy2_init.svg");
	// move sites to centroid of points within ball
	sites = centroid(partition);
	print_to_svg(partition, sites, "swamy2_result.svg");

	// no longer relevant for algorithm, but nice and complete graphical output:
	assign(sites);
	print_to_svg(partition, sites, "swamy2_final.svg");
}

void KMeans::swamyk(int k) {
	Pointset sites = swamyk_sampling(k);
	// assign customers within ball to one of the two sites
	assign_ball(sites);
	print_to_svg(partition, sites, "swamy2_init.svg");
	// move sites to centroid of points within ball
	sites = centroid(partition);
	print_to_svg(partition, sites, "swamy2_result.svg");

	// no longer relevant for algorithm, but nice and complete graphical output:
	assign(sites);
	print_to_svg(partition, sites, "swamy2_final.svg");
}



Pointset KMeans::swamy2_sampling() {
	std::srand(unsigned(std::time(0)));
	Point cen = centroid(customers);
	d1 = eucl2dist(customers, cen);

	double random1 = drand();
	double random2 = drand();

	double sum = 0.;
	Pointset sites = Pointset();
	for (auto c : customers) {
		sum += swamy2_probability_first(c, cen);
		if (sum > random1) {
			sites.push_back(c);
			sites[0].ID = 1;
			break;
		}
	}
	sum = 0.;
	for (auto c : customers) {
		sum += swamy2_probability_second(sites[0],c, cen);
		if (sum > random2) {
			sites.push_back(c);
			sites[1].ID = 2;
			break;
		}
	}

	return sites;
}

Pointset KMeans::swamyk_sampling(int k) {
	Pointset sites = swamy2_sampling();

	std::vector<double> probability(customers.size(), std::numeric_limits<int>::max());
	double probability_sum = 0.;
	// init probability
	for (int i = 0; i < customers.size();++i) {
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

		for (int i = 0; i < customers.size();++i) {
			sum += probability[i]/probability_sum;
			if (sum > rand) {
				break;
			}
			sites.back() = customers[i];
		}
		sites.back().ID = i+1;
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

double KMeans::swamy2_probability_first(Point p, Point centroid) {
	// fancy math stuff as described in the paper
	return ((d1 + (double)n*eucl2dist(p, centroid)) / (2.*(double)n*d1));
}

double KMeans::swamy2_probability_second(Point first, Point p, Point cen) {
	// fancy math stuff as described in the paper
	return (eucl2dist(first, p)/(d1+(double)n*eucl2dist(cen,first)));
}

double KMeans::drand() {
	double d = 0.;
#ifdef _WIN32
	d = (double)rand() / (double)RAND_MAX;
#else
	random2 = drand48();
#endif
	return d;
}

void KMeans::kmeansstep(Pointset& customers, Pointset& sites) {
	assign(sites);
	sites = centroid(partition);
}

// choose (0,0), (1,1), (2,2) as init sites
Pointset KMeans::seed_static(int n) {
	Pointset centers;
	for (int i = 1; i <= n; ++i) {
		centers.push_back(Point(i, i, i));
	}
	return centers;
}

// select a random subset of the customers as init sites
Pointset KMeans::seed_random_subset(int num_of_sites) {
	std::srand(unsigned(std::time(0)));
	std::random_shuffle(customers.begin(), customers.end());
	Pointset centers;
	for (int i = 1; i <= num_of_sites; ++i) {
		customers[i].ID = i;
		centers.push_back(customers[i]);
	}
	return centers;
}

void KMeans::seed_static_and_run() {
	//Pointset sites = seed_static(2);
	//Pointset sites = seed_random_subset(customers, 3);
	Pointset sites = swamy2_sampling();
	assign(sites);
	print_to_svg(partition, sites, "init.svg");
	run(sites, 5);
	print_to_svg(partition, sites, "result.svg");
}

void KMeans::run(Pointset& sites, int steps) {
	for (int i = 0; i < steps; ++i) {
		kmeansstep(customers, sites);
		print_to_svg(partition, sites, std::to_string(i).append(".svg"));
	}
}