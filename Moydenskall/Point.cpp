#include "Point.hpp"
#include <iostream>

Point::Point(double _x, double _y) {
	X = _x;
	Y = _y;
}

std::ostream& operator<<(std::ostream& stream, const Point& p) {
	stream << p.X << " " << p.Y;
	return stream;
}

