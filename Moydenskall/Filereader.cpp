#include "Filereader.h"

Filereader::Filereader(std::string filename) {
	file.open(filename);
	readfile();
}

void Filereader::readfile() {
	

	std::string line = "";

	while (line != "NODE_COORD_SECTION") {
		file >> line;
	}
	file >> line;
	
	double x, y;
	int counter = 1;
	for (std::string k = line; k!="EOF"; file>>k) {
		file >> x >> y;
		Point p(x, y, counter++);
		plane.push_back(p);
	}
}
