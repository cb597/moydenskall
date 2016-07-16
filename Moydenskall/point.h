#ifndef POINT_HPP
#define POINT_HPP
#include <iostream>
class Point {
public:
	Point(double _x, double _y) : x_(_x), y_(_y) {};
	double x() const { return x_; };
	double y() const { return y_; };
	void set_x(double x) { x_ = x; };
	void set_y(double y) { y_ = y; };
	void move_x(double x) { x_ += x; };
	void move_y(double y) { y_ += y; };
private:
	double x_;
	double y_;
};
#endif