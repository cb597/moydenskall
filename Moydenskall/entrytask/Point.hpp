#pragma once
#include <iostream>
class Point {
public:
	Point(double, double, int _id = -1);
	inline bool operator==(const Point & rhs) {
		return(X == rhs.X && Y == rhs.Y);
	};
	double X;
	double Y;
	int ID;
	friend std::ostream& operator<<(std::ostream&, const Point&);
private:

};