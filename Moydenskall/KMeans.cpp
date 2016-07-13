#include "kmeans.hpp"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>
#include "Seeder.hpp"
#include "Partition.hpp"
#include "Instance.hpp"
#include <tuple>
#include <math.h>

KMeans::KMeans(Instance& _instance){
	instance = _instance;
	p = Partition(&(_instance.customers));
	customers = _instance.customers;
}

// run a given seeding strategy and a single ball-k-means step
void KMeans::swamy(const Seeder& seeder) {
	Pointset sites = seeder.seed();
	p.createNewPartition(sites);
	if (instance.svg_ouput) p.print_to_svg(sites, seeder.toString() + "_init.svg");
	// cluster customers within ball to one of the two sites
	// and move sites to centroid of points within ball
	sites = p.ballkmeans(sites);
	if (instance.svg_ouput) p.print_to_svg(sites, seeder.toString() + "_result.svg");
}

// seed and run lloyds algo until capacity limit is repected
double KMeans::lloyds_algo(const Seeder& seeder, unsigned int capacity_limit, double fixed_costs, std::string filenamesuffix) {

	//seed
	Pointset sites = seeder.seed();
	kmeansstep(sites);
	p.createNewPartition(sites);
	if (instance.svg_ouput) p.print_to_svg(sites, "lloyd_" + seeder.toString() + "_init.svg");

	// loop while largest partition violates capacity constraint
	std::tuple<unsigned int, unsigned int> largest_partition = p.get_largest_partition();
	while (std::get<1>(largest_partition) > capacity_limit) {

		//duplicate site
		Point& site_to_duplicate = sites[std::get<0>(largest_partition)];
		Point new_site = Point(site_to_duplicate.X, site_to_duplicate.Y);

		//move slightly to get nonempty voronoi region
		new_site.X = std::nextafter(new_site.X, new_site.X + 42);
		sites.push_back(new_site);

		//single kmeansstep and re-evaluation
		kmeansstep(sites);
		p.createNewPartition(sites);
		largest_partition = p.get_largest_partition();
	}
	//final output
	if (instance.svg_ouput) p.print_to_svg(sites, "lloyd_" + seeder.toString() + "_final"+filenamesuffix+".svg");
	return p.evaluation(fixed_costs);
}

int KMeans::run_lloyd_all_k() {
	//Sample2Seeder swamy2(instance);
	//SampleKSeeder swamyk(instance, 5);
	//StaticSeeder stat5(instance, 5);
	//SubsetSeeder subset(instance, 5);
	//GreedyDelSeeder gredel(instance, 5);
	//LTSeeder lseed(instance, 10);
	//DSeeder dseed(instance, 10);
	//ESeeder eseed(instance, 5);

	unsigned int startk = (unsigned int)std::ceil(instance.D / instance.u);
	unsigned int lg = (unsigned int)std::log(instance.D);
	std::vector<double> results = std::vector<double>();
	for (unsigned int k = startk; k <= lg + startk; ++k) {
		ESeeder eseed = ESeeder(instance, k);
		results.push_back(lloyds_algo(eseed, instance.u, instance.f, std::to_string(k)));
	}

	//get best result
	unsigned int best_id = std::distance(results.begin(), std::min_element(results.begin(), results.end()));
	return best_id;
}

void KMeans::kmeansstep(Pointset& sites) {
	p.createNewPartition(sites);
	sites = p.centroids();
}

void KMeans::seed_and_run(const Seeder& seeder) {
	Pointset sites = seeder.seed();
	p.createNewPartition(sites);
	if (instance.svg_ouput) p.print_to_svg(sites, seeder.toString()+"init.svg");
	run_kmeans(sites, 5);
	if (instance.svg_ouput) p.print_to_svg(sites, seeder.toString() + "result.svg");
	p.print_to_console(sites, instance);
}

void KMeans::run_kmeans(Pointset& sites, int steps) {
	for (int i = 0; i < steps; ++i) {
		kmeansstep(sites);
		if (instance.svg_ouput) p.print_to_svg(sites, std::to_string(i).append(".svg"));
	}
}