# Moydenskall
(anagram for Lloyd's k-means)

this page currently refers to commit 3c2fcf99ab3a6fdee37d1e90871752dbfa14e2d0 (07.07.2016)
###Project Moydenskall

This project will solve instances of the facility location problem as stated in http://www.or.uni-bonn.de/~held/praktikum/facility/aufgabe.pdf

###Compile hints
developed and tested with Microsoft Visual C++ Compiler  
tested with g++ and clang++3.5 (Ubuntu 14.04)

compile command for linux systems:  
`g++ ./*.cpp ./*.hpp -o moydenskall -O3 -std=c++11`

`clang++-3.5 ./*.cpp ./*.hpp -std=c++11 -O3`

###Execution and command line parameter
sample execution for both linux and windows systems:  
`./moydenskall instance.tsp -f 50 -u 5`

#### Synopsis
`./programname <filename> [options]`
filename is an instance in the tsplib format
#### Options
* `-f <int>` double indicating fixed site costs (optional parameter, default: 0)
* `-u <int>` facility capacity, max number of customers per facility  (optional parameter, default: 0)
* `-time <bool>` turn time measurement on and off  (optional parameter, default: false)
* `-svg <bool>` create svg visualization in result.svg (optional parameter, default: false)
* [soon to be implemented: `-w <double>` omega parameter for centroid estimation (optional parameter, default: ??) ]

###Code Analysis
* g++ with -Wall -Wextra --pedantic flags : no warnings
* Valgrind: `All heap blocks were freed -- no leaks are possible`

## Source Overview
* `main.cpp` starting point of the application  
* `Instance.cpp` represents an instance of a kmeans problem. Usually initialised by file in tsp format and other CLI parameters
* `Point.cpp` representation of points (x,y coordinates, ID)
* `Partition.cpp` represents a map of customers to sites and contains many functions to create such a map and provide useful data on this map
* `Seeder.cpp` abstract class and several derived classes of possible seeding strategies for a kmeans algorithm
* `KMeans.cpp` makes use of instance parameters and seeder objects to run kmeans



