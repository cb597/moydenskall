#include "Point.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& stream, const Point& p) {
	stream << p.X << " " << p.Y;
	return stream;
}

