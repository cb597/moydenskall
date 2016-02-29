# moydenskall

Project Moydenskall
-------------------

This project will solve instances of the facility location problem as stated in http://www.or.uni-bonn.de/~held/praktikum/facility/aufgabe.pdf
This will include the following approaches:
1) enumeration algorithm for finding perfect solutions in exponential time
2) Lloyd's k-means algorithm (approximative solutions)
3) diverse approaches to find starting seeds for Lloyd's algorithm

Current status:
1) finished
2) not yet started
3) not yet started



Compile hints
-------------------
developed and tested with Microsoft Visual C++ Compiler
tested with g++

compile command for linux systems:
g++ ./* -o moydenskall -O3 -std=C++11



Execution and command line parameter
-------------------
for both linux and windows systems:
./moydenskall instance.tsp 5

the value 5 can be replaced with any integer indicating fixed site costs
