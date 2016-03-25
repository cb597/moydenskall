#include <vector>
#include <iostream>
#include <algorithm>
#include "Point.h"
#include "Enumerator.h"
#include "Tools.h"
#include <limits>
#include <fstream>
#include <math.h>

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

Enumerator::Enumerator(double f, int u) {
	best_costs = std::numeric_limits<double>::max();
	fix_costs = f;
	capacity = u;
}

void Enumerator::create_partition(std::vector<Plane>& partition, Plane& left) {
	if (left.size() == 0) {
		Plane cen = centroid(partition);
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
	for (int i = 0; i < partition.size(); ++i) {
		if (partition[i].size() < capacity) { //only if <capacity
			partition[i].push_back(current);
			create_partition(partition, left);
			partition[i].pop_back();
		}
	}

	// deal with case: new subset for current element
	partition.push_back(Plane());
	partition.back().push_back(current);
	create_partition(partition, left);
	partition.pop_back();

	// restore
	left.push_back(current);
}

void Enumerator::print_result(bool svg) {
	std::cout << "OBJECTIVE" << std::endl << best_costs << std::endl;
	for (auto cen : best_sites) {
		std::cout << "FACILITY " << cen << std::endl;
	}
	for (int part = 0; part < best_partition.size(); ++part) {
		for (auto point : best_partition[part]) {
			std::cout << "ASSIGN " << point.ID << " " << part+1 << std::endl;
		}
	}

	if (svg) {
		svg_output();
	}
}

void Enumerator::svg_output() {
	std::ofstream svgfile("result.svg");

	double xmin = 99999, ymin = 99999;
	double xmax = 0, ymax = 0;

	for (auto part : best_partition) {
		for (auto p : part) {
			xmin = std::min(xmin, p.X);
			xmax = std::max(xmax, p.X);
			ymin = std::min(ymin, p.Y);
			ymax = std::max(ymax, p.Y);
		}
	}

	xmin -= 2; ymin -= 2; xmax += 2; ymax += 2;
	double scale = 20;
	int pointsize = ceil(std::max((xmax-xmin)/10.,(ymax-ymin)/10.));

	svgfile << "<?xml version=\"1.0\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"	\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
	svgfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"960\" height=\"700\" viewBox = \""
		<< scale*xmin << " " << scale*ymin << " " << scale*(xmax - xmin) << " " << scale*(ymax - ymin)
		<< "\" preserveAspectRatio = \"xMidYMid meet\">" << std::endl;
	//svgfile << "<rect x = \"0\" y = \"0\" width = \"500\" height = \"500\" style = \"stroke:black;stroke-width:3;fill:white\"/>" << std::endl;




	for (int part = 0; part < best_partition.size(); ++part) {
		auto cen = best_sites[part];
		svgfile << "<circle cx = \"" << scale * cen.X << "\" cy = \"" << scale * cen.Y << "\" r = \""
			<< pointsize << "\" style = \"fill:green\" />" << std::endl;
		for (auto p : best_partition[part]) {
			svgfile << "<line x1=\"" << scale * p.X << "\" y1=\"" << scale*p.Y << "\" x2=\"" << scale * cen.X << "\" y2=\"" << scale * cen.Y
				<< "\" style=\"stroke:#aaa; stroke-width:"<< pointsize <<"; \" stroke-dasharray=\"1, 1\"/>";
			svgfile << "<circle cx = \"" << scale * p.X << "\" cy = \"" << scale*p.Y << "\" r = \""
				<< pointsize << "\" style = \"fill:red\" />" << std::endl;
		}
	}


	svgfile << "</svg>";
}
