#pragma once
#include "Point.h"
#include <vector>
typedef std::vector<Point> Plane;



Point centroid(const Plane&);
Plane centroid(const std::vector<Plane>&);
Plane create_demo_plane();