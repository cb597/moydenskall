#include "Tools.h"

Point centroid(const Plane& plane) {

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




Plane create_demo_plane() {
	Plane plane;
	plane.push_back(Point(0., 1.));
	plane.push_back(Point(0., 2.));
	plane.push_back(Point(0., 3.));
	plane.push_back(Point(0., 4.));
	return plane;
}