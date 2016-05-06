#include "Tools.hpp"
#include <fstream>
#include <string>
#include <algorithm>

Point centroid(const Plane& plane) {
	if (plane.size() == 0) {
		return(Point(0, 0));
	}
	double x = 0.;
	double y = 0.;
	for (auto p : plane) {
		x += p.X;
		y += p.Y;
	}

	x /= plane.size();
	y /= plane.size();

	return Point(x,y);
}

Plane centroid(const std::vector<Plane>& partition) {
	Plane centroids;
	int counter = 1;
	for (auto plane : partition) {
		centroids.push_back(centroid(plane));
		centroids.back().ID = counter++;
	}
	return centroids;
}

double eucl2dist(Plane plane, Point site) {
	double sum = 0.0;
	for (Point p : plane) {
		sum += eucl2dist(site, p);
	}
	return sum;
}

double eucl2dist(Point a, Point b) {
	double xdiff = a.X - b.X;
	double ydiff = a.Y - b.Y;
	return xdiff*xdiff + ydiff*ydiff;
}

double evaluate_partition(Partition partition, Plane sites, double fix_costs) {
	if (sites.size() != partition.size()) {
		throw "incompatible amount of sites and partitions";
	}

	double sum = sites.size() * fix_costs;
	for (unsigned int i = 0; i < sites.size(); ++i) {
		sum += eucl2dist(partition[i], sites[i]);
	}

	return sum;
}


Plane readfile(std::string filename) {
	
	std::ifstream file(filename);
	std::string line = "";
	Plane plane;

	while (line != "NODE_COORD_SECTION") {
		file >> line;
	}
	file >> line;

	double x, y;
	int counter = 1;
	for (std::string k = line; k != "EOF"; file >> k) {
		file >> x >> y;
		Point p(x, y, counter++);
		plane.push_back(p);
	}

	return plane;
}

void print_to_svg(Partition partition, Plane sites, std::string filename) {
	std::ofstream svgfile(filename);

	double xmin = 99999, ymin = 99999;
	double xmax = 0, ymax = 0;

	for (auto part : partition) {
		for (auto p : part) {
			xmin = std::min(xmin, p.X);
			xmax = std::max(xmax, p.X);
			ymin = std::min(ymin, p.Y);
			ymax = std::max(ymax, p.Y);
		}
	}
	for (auto s : sites) {
		xmin = std::min(xmin, s.X);
		xmax = std::max(xmax, s.X);
		ymin = std::min(ymin, s.Y);
		ymax = std::max(ymax, s.Y);
	}

	xmin -= 2; ymin -= 2; xmax += 2; ymax += 2;
	double scale = 20;
	int pointsize = (int)ceil(std::max((xmax - xmin) / 10., (ymax - ymin) / 10.));

	svgfile << "<?xml version=\"1.0\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"	\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
	svgfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"960\" height=\"700\" viewBox = \""
		<< scale*xmin << " " << scale*ymin << " " << scale*(xmax - xmin) << " " << scale*(ymax - ymin)
		<< "\" preserveAspectRatio = \"xMidYMid meet\">" << std::endl;
	//svgfile << "<rect x = \"0\" y = \"0\" width = \"500\" height = \"500\" style = \"stroke:black;stroke-width:3;fill:white\"/>" << std::endl;




	for (unsigned int part = 0; part < partition.size(); ++part) {
		auto cen = sites[part];
		svgfile << "<circle cx = \"" << scale * cen.X << "\" cy = \"" << scale * cen.Y << "\" r = \""
			<< pointsize << "\" style = \"fill:green\" />" << std::endl;
		for (auto p : partition[part]) {
			svgfile << "<line x1=\"" << scale * p.X << "\" y1=\"" << scale*p.Y << "\" x2=\"" << scale * cen.X << "\" y2=\"" << scale * cen.Y
				<< "\" style=\"stroke:#aaa; stroke-width:" << pointsize << "; \" stroke-dasharray=\"1, 1\"/>";
			svgfile << "<circle cx = \"" << scale * p.X << "\" cy = \"" << scale*p.Y << "\" r = \""
				<< pointsize << "\" style = \"fill:red\" />" << std::endl;
		}
	}


	svgfile << "</svg>";
}
