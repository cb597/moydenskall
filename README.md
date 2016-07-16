# Moydenskall
(anagram for Lloyd's k-means)

this page currently refers to commit bcb512adfdf44ec7ad7d3fda72a7278df8a0cfda (08.07.2016)
###Project Moydenskall

This project will solve instances of the facility location problem as stated in http://www.or.uni-bonn.de/~held/praktikum/facility/aufgabe.pdf

Almost all implemented algorithms of this project are described and analyzed in this paper:
https://www.math.uwaterloo.ca/~cswamy/papers/kmeans-journ.pdf

###Compile hints
developed and tested with Microsoft Visual C++ Compiler  
tested with g++ and clang++3.5 (Ubuntu 14.04)

compile command for linux systems:  
`g++ ./*.cc ./*.h -o moydenskall -std=c++11`

`clang++-3.5 ./*.cc ./*.h -std=c++11`

###Execution and command line parameter
sample execution for both linux and windows systems:  
`./moydenskall instances/instance.tsp -f 50 -u 5`

#### Synopsis
`./programname <filename> [options]`
filename is an instance in the tsplib format
#### Options
* `-f <int>` double indicating fixed site costs (optional parameter, default: 0)
* `-u <int>` facility capacity, max number of customers per facility  (optional parameter, default: 0)
* `-time <bool>` turn time measurement on and off  (optional parameter, default: false)
* `-svg <bool>` create svg visualization in result.svg (optional parameter, default: false)
* `-w <double>` omega parameter for centroid estimation (optional parameter, default: ??)
* `-k <int>` use algorithm to only run with a given k (turn off trying several k, good for quick test runs)

###Code Analysis
* g++ with -Wall -Wextra --pedantic flags : no warnings
* Valgrind: `All heap blocks were freed -- no leaks are possible`

## Source Overview
* `main.cc` starting point of the application  
* `Instance.cc` represents an instance of a kmeans problem. Usually initialised by file in tsp format and other CLI parameters
* `Point.cc` representation of points (x,y coordinates, ID)
* `Partition.cc` represents a map of customers to sites and contains many functions to create such a map and provide useful data on this map
* `Seeder.cc` abstract class and several derived classes of possible seeding strategies for a kmeans algorithm
* `KMeans.cc` makes use of instance parameters and seeder objects to run kmeans



