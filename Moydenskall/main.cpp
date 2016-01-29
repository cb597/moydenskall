//#include "Filereader.cpp"
#include <vector>
#include "Enumerator.h"
#include "Point.h"
#include "Filereader.h"





int main(int argc, char* argv[]) {

	int f = 0;
	std::string instance_filename = "inst1.tsp";

	if (argc >= 2) {
		instance_filename = argv[1];
	}

	if (argc >= 3) {
		f = std::stoi(argv[2]);
	}


	Filereader fr(instance_filename);
	
	Plane p = fr.plane;

	Enumerator en;
	std::vector<Plane> init;
	en.create_partition(init, p);
	return 0;
}