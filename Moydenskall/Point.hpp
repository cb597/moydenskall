#ifndef POINT_HPP
#define POINT_HPP
#include <iostream>
class Point {
public:
	Point(double, double, int _id = -1);
	inline bool operator==(const Point & rhs) {
		return(X == rhs.X && Y == rhs.Y);
	};
	double X;
	double Y;
	friend std::ostream& operator<<(std::ostream&, const Point&);
	int getId();
	void setId(int _id);
private:
	int ID;
};

#endif