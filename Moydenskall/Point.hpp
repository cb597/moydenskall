#ifndef POINT_HPP
#define POINT_HPP
#include <iostream>
class Point {
public:
	Point(double x, double y) : X(x), Y(y) {};
	inline bool operator==(const Point & rhs) {
		return(X == rhs.X && Y == rhs.Y);
	};
	double X;
	double Y;
	friend std::ostream& operator<<(std::ostream&, const Point&);
};

#endif