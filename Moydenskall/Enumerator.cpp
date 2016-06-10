#include <vector>
#include <iostream>
#include <algorithm>
#include "Point.hpp"
#include "Enumerator.hpp"
#include "Tools.hpp"
#include <limits>
#include <fstream>
#include <math.h>

std::ostream& operator<<(std::ostream& stream, const Pointset& plane) {
	stream << "{ ";
	for (auto p : plane) {
		stream << p << (p == plane.back() ? " }" : ", ");
	}
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const std::vector<Pointset>& partition) {
	stream << "{";
	for (auto part : partition) {
		stream << part;
	}
	stream << "}" << std::endl;
	return stream;
}

Enumerator::Enumerator(double f, int u) {
	best_costs = std::numeric_limits<double>::max();
	fix_costs = f;
	capacity = u;
}

void Enumerator::create_partition(std::vector<Pointset>& partition, Pointset& left) {
	if (left.size() == 0) {
		Pointset cen = centroid(partition);
		double costs = evaluate_partition(partition, cen, fix_costs);
		if (costs < best_costs) {
			best_costs = costs;
			best_sites = cen;
			best_partition = partition;
			//std::cout << "current best value = " << costs << std::endl;
		}
		return;
	}

	Point current = left.back();
	left.pop_back();

	// insert into each existing subset
	for (unsigned int i = 0; i < partition.size(); ++i) {
		if ((int) partition[i].size() < capacity) { //only if <capacity
			partition[i].push_back(current);
			create_partition(partition, left);
			partition[i].pop_back();
		}
	}

	// deal with case: new subset for current element
	partition.push_back(Pointset());
	partition.back().push_back(current);
	create_partition(partition, left);
	partition.pop_back();

	// restore
	left.push_back(current);
}

void Enumerator::print_result(bool svg) {
	std::cout << "OBJECTIVE " << best_costs << std::endl;
	for (auto cen : best_sites) {
		std::cout << "FACILITY " << cen << std::endl;
	}
	for (unsigned int part = 0; part < best_partition.size(); ++part) {
		for (auto point : best_partition[part]) {
			std::cout << "ASSIGN " << point.getId() << " " << part+1 << std::endl;
		}
	}

	if (svg) {
		svg_output();
	}
}

void Enumerator::svg_output() {
	//print_to_svg(c best_partition, best_sites, "result.svg");
}
