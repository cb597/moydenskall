#include "Tools.hpp"
#include <fstream>
#include <string>
#include <algorithm>
#include <tuple>
#include "ExtPartition.hpp"

Point centroid(const Pointset& plane) {
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

Pointset centroid(const std::vector<Pointset>& partition) {
	Pointset centroids;
	int counter = 1;
	for (auto plane : partition) {
		centroids.push_back(centroid(plane));
		centroids.back().setId(counter++);
	}
	return centroids;
}

double eucl2dist(Pointset plane, Point site) {
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

double evaluate_partition(Partition partition, Pointset sites, double fix_costs) {
	if (sites.size() != partition.size()) {
		throw "incompatible amount of sites and partitions";
	}

	double sum = sites.size() * fix_costs;
	for (unsigned int i = 0; i < sites.size(); ++i) {
		sum += eucl2dist(partition[i], sites[i]);
	}

	return sum;
}


std::vector<double> evaluate_partition(std::tuple<Partition, std::vector<int> > partition, Pointset sites) {
	if (sites.size() != std::get<0>(partition).size()) {
		throw "incompatible amount of sites and partitions";
	}

	std::vector<double> stderrors = std::vector<double>();
	for (unsigned int i = 0; i < sites.size(); ++i) {
		stderrors.push_back(eucl2dist(std::get<0>(partition)[i], sites[i]));
	}

	std::vector<double> errors = std::vector<double>();

	return errors;
}
Partition cluster(const Pointset& customers, const Pointset& sites) {
	ExtPartition part = ExtPartition(&customers, sites);
	return part.getOldPartition(customers);
}

Pointset readfile(std::string filename) {
	
	std::ifstream file(filename);
	std::string line = "";
	Pointset input_points;

	while (line != "NODE_COORD_SECTION") {
		file >> line;
	}
	file >> line;

	double x, y;
	int counter = 1;
	for (std::string k = line; k != "EOF"; file >> k) {
		file >> x >> y;
		Point p(x, y, counter++);
		input_points.push_back(p);
	}

	return input_points;
}

void print_to_svg(Pointset customers, Partition partition, Pointset sites, std::string filename) {
	std::ofstream svgfile(filename);

	double xmin = 99999, ymin = 99999;
	double xmax = 0, ymax = 0;

	for (auto c : customers) {
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
	//svgfile << "<rect x = \"0\" y = \"0\" width = \"500\" height = \"500\" style = \"stroke:black;stroke-width:3;fill:white\"/>" << std::endl;




	for (unsigned int part = 0; part < partition.size(); ++part) {
		auto cen = sites[part];
		svgfile << "<circle cx = \"" << cen.X << "\" cy = \"" <<  cen.Y << "\" r = \""
			<< pointsize << "\" style = \"fill:green\" />" << std::endl;
		for (auto p : partition[part]) {
			svgfile << "<line x1=\"" <<  p.X << "\" y1=\"" << p.Y << "\" x2=\"" <<  cen.X << "\" y2=\"" <<  cen.Y
				<< "\" style=\"stroke:#aaa; stroke-width:" << pointsize << "; \" stroke-dasharray=\"0.1, 0.1\"/>";
			svgfile << "<circle cx = \"" << p.X << "\" cy = \"" << p.Y << "\" r = \""
				<< pointsize << "\" style = \"fill:red\" />" << std::endl;
		}
	}

	double sum = 0;
	for (unsigned int i = 0; i < partition.size(); ++i) {
		sum += eucl2dist(partition[i], sites[i]);
	}
	svgfile << "<text x = \""<< xmin <<"\" y = \""<< ymin <<"\" fill = \"black\"  style=\"font-size:"<<3*pointsize<<"px\">" << filename << " - " << sum << "</text>\n";
	svgfile << "</svg>";
}


double drand() {
	double d = 0.;
#ifdef _WIN32
	d = (double)rand() / (double)RAND_MAX;
#else
	d = drand48();
#endif
	return d;
}