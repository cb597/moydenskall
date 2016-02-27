#include <vector>
#include <iostream>
#include <algorithm>
#include "Point.h"
#include "Enumerator.h"
#include "Tools.h"
#include <limits>

std::ostream& operator<<(std::ostream& stream, const Plane& plane) {
	stream << "{ ";
	for (auto p : plane) {
		stream << p << (p == plane.back() ? " }" : ", ");
	}
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const std::vector<Plane>& partition) {
	stream << "{";
	for (auto part : partition) {
		stream << part;
	}
	stream << "}" << std::endl;
	return stream;
}

Enumerator::Enumerator(double f) {
	best_costs = std::numeric_limits<double>::max();
	eval.set_fix_costs(f);
}

void Enumerator::create_partition_of_n(std::vector<int>& until, int last, int left) {
	if (left == 0) {
		return;
	}
	for (int next = std::min(last, left); next > 0; --next) {
		until.push_back(next);
		create_partition_of_n(until, next, left - next);
		until.pop_back();
	}

}

void Enumerator::create_partition(std::vector<Plane>& partition, Plane& left) {
	if (left.size() == 0) {
		Plane cen = centroid(partition);
		double costs = eval.evaluate(partition, cen);
		if (costs < best_costs) {
			best_costs = costs;
			best_sites = cen;
			std::cout << "current best value = " << costs << std::endl;
		}
		return;
	}

	Point current = left.back();
	left.pop_back();

	// insert into each existing subset
	for (int i = 0; i < partition.size(); ++i) {
		partition[i].push_back(current);
		create_partition(partition, left);

		partition[i].pop_back();
	}

	// deal with case: new subset for current element
	partition.push_back(Plane());
	partition.back().push_back(current);
	create_partition(partition, left);
	partition.pop_back();

	// restore
	left.push_back(current);
}

void Enumerator::print_result() {
	std::cout << "best result: " << std::endl;
	for (auto cen : best_sites) {
		std::cout << cen.X << " / " << cen.Y << std::endl;
	}
}
