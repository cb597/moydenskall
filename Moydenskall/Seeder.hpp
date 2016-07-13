#ifndef SEEDER_HPP
#define SEEDER_HPP
#include "Partition.hpp"
#include "Instance.hpp"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>

// abstract class for seeding strategies
class Seeder {
public:
	Seeder(Instance& _instance) : instance(_instance), customers(instance.customers), k(instance.k) {};
	virtual Pointset seed() const = 0;
	virtual std::string toString() const = 0;
protected:
	Instance instance;
	Pointset customers;
	int k; //number of sites to be seeded
};

// trivial demo seeding strategy for testing purposes
class StaticSeeder : public Seeder {
public:
	StaticSeeder(Instance instance) : Seeder(instance) { };
	Pointset seed() const;
	std::string toString() const { return "static_seeder"; }
};

// intuitive seeding strategy: choose a random subset
class SubsetSeeder : public Seeder {
public:
	SubsetSeeder(Instance instance) : Seeder(instance) {};
	Pointset seed() const;
	std::string toString() const { return "subset_seeder"; }
};

// sample algorithm for the 2-means problem (chapter 3)
class Sample2Seeder : public Seeder {
public:
	Sample2Seeder(Instance instance) : Seeder(instance) {};
	Pointset seed() const;
	std::string toString() const { return "Sample2Seeder"; }
};

// Stage I Seeding procedure (described in chapter 4.1.1)
class SampleKSeeder : public Seeder {
public:
	SampleKSeeder(Instance instance) : Seeder(instance) {};
	SampleKSeeder(Instance instance, unsigned int _k) : Seeder(instance) { k = _k; };
	Pointset seed() const;
	std::string toString() const { return "SampleKSeeder"; }
};

// greedy deletion seeder, given a init Pointset delete successively
class GreedyDelSeeder : public Seeder {
public:
	GreedyDelSeeder(Instance instance) : Seeder(instance) {};
	Pointset seed() const;
	Pointset seed(Pointset init) const;
	std::string toString() const { return "GreedyDelSeeder"; }
};

// seeding algo in linear time (chapter 4.1.3)
class LTSeeder : public Seeder {
public:
	LTSeeder(Instance instance) : Seeder(instance) {};
	Pointset seed() const;
	std::string toString() const { return "LTSeeder"; }
};

// linear time constant factor approximation algorithm (chapter 4.3)
class DSeeder : public Seeder {
public:
	DSeeder(Instance instance) : Seeder(instance) {};
	Pointset seed() const;
	std::string toString() const { return "DSeeder"; }
private:
	Pointset ballkmeansstep(Pointset& sites) const;
};

// PTAS (chapter 4.4)
class ESeeder : public Seeder {
public:
	ESeeder(Instance instance) : Seeder(instance) {};
	Pointset seed() const;
	std::string toString() const { return "ESeeder"; }
};

#endif