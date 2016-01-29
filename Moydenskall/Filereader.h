#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Point.h"
typedef std::vector<Point> Plane;

class Filereader {
public:
	Filereader(std::string);
	Plane plane;

private:
	std::ifstream file;
	void readfile();
};