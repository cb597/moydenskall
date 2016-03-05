#include "Point.h"
#include <iostream>

Point::Point(double _x, double _y, int _id) {
	X = _x;
	Y = _y;
	ID = _id;
}


std::ostream& operator<<(std::ostream& stream, const Point& p) {
	stream << p.ID << " " << p.X << " " << p.Y;
	return stream;
}

