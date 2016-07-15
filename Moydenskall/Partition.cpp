#include "Partition.hpp"
#include "Instance.hpp"
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <limits>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <random>
#include <numeric>

double eucl2dist(Point a, Point b) {
	double xdiff = a.X - b.X;
	double ydiff = a.Y - b.Y;
	return xdiff*xdiff + ydiff*ydiff;
}

Partition::Partition(const Pointset* _customers, const Pointset& _sites) : customers(_customers) {
	k = _sites.size();
	setSites(_sites);
}

unsigned int Partition::getMinTx() {
	std::vector<double>::iterator result = std::min_element(std::begin(Tx), std::end(Tx));
	return std::distance(std::begin(Tx), result);
}

unsigned int Partition::getMinWeightedTx() {
	double min_val = std::numeric_limits<double>::max();
	unsigned int min_idx = 0;
	for (unsigned int i = 0; i < Tx.size(); ++i) {
		if (Tx[i] * partition_size[i] < min_val) {
			min_idx = i;
		}
	}
	return min_idx;
}

unsigned int Partition::assigned(unsigned int idx) {
	return id_1best[idx];
}

unsigned int Partition::assigned_alternative(unsigned int idx) {
	return id_2best[idx];
}

void Partition::delete_partition(unsigned int idx) {
	for (unsigned int i = 0; i < id_1best.size(); ++i) {
		//reassign points assigned to the partition about to be deleted
		if (id_1best[i] == idx) {
			id_1best[i] = id_2best[i];
		}
		if (id_2best[i] == idx) {
			id_2best[i] = std::numeric_limits<unsigned int>::quiet_NaN();
		}

		//adjust indices as a partition is missing now
		if (id_1best[i] > idx) --id_1best[i];
		if (id_2best[i] > idx) --id_2best[i];
	}
	--k;
}

Pointset Partition::centroids() {
	std::vector<Point> centers(k, Point(0,0));
	std::vector<unsigned int> counter(k, 0);
	for (unsigned int i = 0; i < (*customers).size(); ++i) {
		if (assigned(i) == k) continue;
		centers[assigned(i)].X += (*customers)[i].X;
		centers[assigned(i)].Y += (*customers)[i].Y;
		counter[assigned(i)]++;
	}
	for (unsigned int i = 0; i < k; ++i) {
		centers[i].X /= counter[i];
		centers[i].Y /= counter[i];
	}
	return centers;
}

// ball-k-means step as described in 4.2 (A) 
void Partition::ballkmeans() {
	std::vector<double> ddach(k, std::numeric_limits<double>::infinity());
	for (unsigned int i = 0; i < sites.size() - 1; ++i) {
		for (unsigned int j = i + 1; j < sites.size(); ++j) {
			double ndist = eucl2dist(sites[i], sites[j]);
			ddach[i] = std::min(ddach[i], ndist);
			ddach[j] = std::min(ddach[j], ndist);
		}
	}

	for (unsigned int i = 0; i < id_1best.size();++i) {
		id_1best[i] = k;
	}
	
	for (unsigned int c = 0; c < (*customers).size();++c) {
		for (unsigned int s = 0; s < sites.size(); ++s) {
			if (eucl2dist(sites[s], (*customers)[c]) < ddach[s] / 3) {
				id_1best[c] = s;
			}
		}
	}
	sites = centroids();
	return;
}

// calculate Voronoi regions and errors 
void Partition::setSites(const Pointset& _sites) {
	sites = _sites;
	k = sites.size();

	//init all vectors
	id_1best = std::vector<unsigned int>((*customers).size(), 0);
	val_1best = std::vector<double>((*customers).size(), std::numeric_limits<int>::max());
	id_2best = std::vector<unsigned int>((*customers).size(), 0);
	val_2best = std::vector<double>((*customers).size(), std::numeric_limits<int>::max());

	// loop over all (*customers) and sites and determine best and secondbest site per customer
	for (unsigned int c = 0; c < (*customers).size(); ++c) {
		for (unsigned int s = 0; s < sites.size(); ++s) {
			double dist = eucl2dist((*customers)[c], sites[s]);
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

	// calculate costs and size of each partition
	partition_size.clear();
	partition_size.resize(k, 0);
	TotalError = 0.; //cost of clustering customers around sites
	Tx = std::vector<double>(sites.size(), 0.); // cost of clustering customers around sites\{x}
	for (unsigned int i = 0; i < (*customers).size(); i++) {
		TotalError += val_1best[i];
		for (unsigned int t = 0; t < sites.size(); ++t) {
			Tx[t] += t != id_1best[i] ? val_1best[i] : val_2best[i];
		}
		++partition_size[assigned(i)];
	}

}

void Partition::print_to_svg(std::string filename) {
	std::ofstream svgfile(filename);

	// get bounding rectangle

	double xmin = 99999, ymin = 99999;
	double xmax = 0, ymax = 0;

	for (auto c : (*customers)) {
		xmin = std::min(xmin, c.X);
		xmax = std::max(xmax, c.X);
		ymin = std::min(ymin, c.Y);
		ymax = std::max(ymax, c.Y);
	}
	for (auto s : sites) {
		xmin = std::min(xmin, s.X);
		xmax = std::max(xmax, s.X);
		ymin = std::min(ymin, s.Y);
		ymax = std::max(ymax, s.Y);
	}

	xmin -= 2; ymin -= 2; xmax += 2; ymax += 2;
	double pointsize = (double)std::max((double)(xmax - xmin) / 100., (double)(ymax - ymin) / 600.);

	svgfile << "<?xml version=\"1.0\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"	\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
	svgfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"900\" height=\"700\" viewBox = \""
		<< xmin << " " << ymin << " " << (xmax - xmin) << " " << (ymax - ymin)
		<< "\" preserveAspectRatio = \"xMidYMid meet\">" << std::endl;

	for (unsigned int c = 0; c < (*customers).size(); ++c) {
		//draw line to assigned site
		svgfile << "<line x1=\"" << (*customers)[c].X << "\" y1=\"" << (*customers)[c].Y << "\" x2=\"" << sites[assigned(c)].X << "\" y2=\"" << sites[assigned(c)].Y
			<< "\" style=\"stroke:#aaa; stroke-width:" << pointsize/2 << "; \" stroke-dasharray=\"0.1, 0.1\"/>";
	}
	for (unsigned int c = 0; c < (*customers).size(); ++c) {
		//draw customer
		svgfile << "<circle cx = \"" << (*customers)[c].X << "\" cy = \"" << (*customers)[c].Y << "\" r = \""
			<< pointsize << "\" style = \"fill:red\" />" << std::endl;
	}
	for (unsigned int s=0; s < sites.size(); ++s) {
		//draw site
		svgfile << "<circle cx = \"" << sites[s].X << "\" cy = \"" << sites[s].Y << "\" r = \""
			<< pointsize << "\" style = \"fill:green\" />" << std::endl;
	}

	svgfile << "<text x = \"" << xmin << "\" y = \"" << ymin << "\" fill = \"black\"  style=\"font-size:" << 3 * pointsize << "px\">" << filename << " - " << TotalError << "</text>\n";
	svgfile << "</svg>";
}

void Partition::print_to_console(const Instance& instance) {
	std::cout << "OBJECTIVE " << evaluation(instance.f) << std::endl;
	for (unsigned int s = 0; s < sites.size(); ++s) {
		std::cout << "FACILITY " << s << " " << sites[s].X << " " << sites[s].Y << std::endl;
	}
	std::cout << "ASSIGN " << std::endl;
	for (unsigned int c = 0; c < (*customers).size(); ++c) {
		std::cout << c << " " << assigned(c) << std::endl;
	}
}

Point Partition::centroid(Pointset& points) const {
	if (points.size() == 0) {
		return(Point(0, 0));
	}
	double x = 0.;
	double y = 0.;
	for (auto p : points) {
		x += p.X;
		y += p.Y;
	}

	x /= points.size();
	y /= points.size();

	return Point(x, y);
}

//recursive function to determine all subsetsand get their centroids
void Partition::subsetcentroids(Pointset& result, Pointset& set, Pointset& chosen, unsigned int position, unsigned int left) const {
	if (left == 0) {
		result.push_back(centroid(chosen));
		return;
	}
	else if (set.size() - position < left) {
		//cut this recursion branch if not enough items left
		return;
	}
	else {
		chosen.push_back(set[position]);
		subsetcentroids(result, set, chosen, position + 1, left - 1);
		chosen.pop_back();
		subsetcentroids(result, set, chosen, position + 1, left);
	}
}

// recursive function to select best elements of candidates, used in centroid_estimation
double Partition::get_optimal_candidates(std::vector<Pointset>& candidates, Pointset& chosen, unsigned int cur_part, double bestval, Pointset& bestset) {
	if (cur_part == candidates.size()) {
		Partition part = Partition(customers, chosen);
		if (part.TotalError < bestval) {
			bestval = part.TotalError;
			bestset.clear();
			for (unsigned int i = 0; i < chosen.size(); ++i) {
				bestset.push_back(chosen[i]);
			}
		}
		return bestval;
	}
	for (unsigned int i = 0; i < candidates[cur_part].size(); ++i) {
		chosen.push_back(candidates[cur_part][i]);
		bestval = get_optimal_candidates(candidates, chosen, cur_part + 1, bestval, bestset);
		chosen.pop_back();
	}
	return bestval;
}

Pointset Partition::centroid_estimation(Pointset& init_centers, double omega, double eps) {

	double beta = 25 / (25 + 256 * eps*eps);

	// define ddach(i) = min_{j!=i}(||c_j = c_i||) where c_i are init_centers
	std::vector<double> ddach = std::vector<double>(k, std::numeric_limits<double>::infinity());
	for (unsigned int i = 0; i < k; ++i) {
		for (unsigned int j = i+1; j < k; ++j) {
			ddach[i] = std::min(ddach[i], eucl2dist(init_centers[i], init_centers[j]));
			ddach[j] = std::min(ddach[j], eucl2dist(init_centers[i], init_centers[j]));
		}
	}

	//calculate expanded voronoi regions
	std::vector<Pointset> expanded_voronoi_regions = std::vector<Pointset>(init_centers.size(), Pointset());
	for (unsigned int x = 0; x < (*customers).size(); ++x) {
		for (unsigned int region_index = 0; region_index < k; ++region_index) {
			if (eucl2dist((*customers)[x], init_centers[region_index]) <= eucl2dist((*customers)[x], (*customers)[assigned(x)]) + ddach[region_index] / 4) {
				expanded_voronoi_regions[region_index].push_back((*customers)[x]);
			}
		}
	}

	//random subset
	std::vector<Pointset> random_subsets; // denoted as "S" in paper
	unsigned int amount = (int)(4 / (beta*omega));
	for (auto region : expanded_voronoi_regions) {
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
	std::vector<Pointset> candidates;
	for (auto s : random_subsets) {
		unsigned int subsetsize = (int)(2 / omega);
		Pointset result = Pointset();
		Pointset chosen = Pointset();
		subsetcentroids(result, s, chosen, 0, subsetsize);
		candidates.push_back(result);
	}


	// select optimal candidate from each s \in subsetcentroid
	Pointset chosen = Pointset();
	Pointset bestset = Pointset();
	get_optimal_candidates(candidates, chosen, 0, std::numeric_limits<double>::infinity(), bestset);


	return bestset;

}

double Partition::evaluation(double fixed_costs) {
	return TotalError+k*fixed_costs;
}

Pointset Partition::getSites() {
	return sites;
}

bool Partition::capacity_check_and_clone(unsigned int limit) {
	auto max = std::max_element(partition_size.begin(), partition_size.end());
	if (*max > limit) {
		Point& to_duplicate = sites[std::distance(partition_size.begin(), max)];
		Point new_point = Point(to_duplicate.X, to_duplicate.Y);

		//move slightly to get nonempty voronoi region
		new_point.X += 1;// std::nextafter(new_point.X, new_point.X + 42);
		Pointset newsites = sites;
		newsites.push_back(new_point);
		setSites(newsites);
		//++k;
		return true;
	}
	return false;
}
