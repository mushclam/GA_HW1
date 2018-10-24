#pragma once
#include "pch.h"
#include "Individual.h"

#ifndef POPULATION_H
#define POPULATION_H

class Population
{
public:
	int popSize;
	Individual individual[POPULATION_SIZE];

	Population(int);
	void Initialize();
	void wheelSelection();
	void tournamentSelection();
	void crossover();
	void mutation();
	std::string evaluation(_itemSet[]);
};

#endif // !POPULATION_H