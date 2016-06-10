#ifndef Seeder_hpp
#define Seeder_hpp
#include "kmeans.hpp"
#include <limits>
#include <algorithm>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <random>

class Seeder {
public:
	Seeder(const Pointset& _customers) { customers = _customers; }
	virtual Pointset seed() const = 0;
	virtual std::string toString() const = 0;
protected:
	Pointset customers;
};

class StaticSeeder : public Seeder {
public:
	StaticSeeder(Pointset _customers) : Seeder(_customers) { n = 2; };
	StaticSeeder(Pointset _customers, int _n) : Seeder(_customers) { n = _n; };
	Pointset seed() const;
	std::string toString() const { return "static_seeder"; }
private:
	int n; //number of sites to be seeded
};


class SubsetSeeder : public Seeder {
public:
	SubsetSeeder(Pointset _customers) : Seeder(_customers) { n = 2; };
	SubsetSeeder(Pointset _customers, int _n) : Seeder(_customers) { n = _n; };
	Pointset seed() const;
	std::string toString() const { return "subset_seeder"; }
private:
	int n; //number of sites to be seeded
};



class Swamy2Seeder : public Seeder {
public:
	Swamy2Seeder(Pointset _customers) : Seeder(_customers) {};
	Pointset seed() const;
	std::string toString() const { return "swamy2_seeder"; }
};


class SwamykSeeder : public Seeder {
public:
	SwamykSeeder(Pointset _customers, int _k) : Seeder(_customers) { k = _k; };
	Pointset seed() const;
	std::string toString() const { return "swamyk_seeder"; }
private:
	int k; // seed k centers
};

#endif