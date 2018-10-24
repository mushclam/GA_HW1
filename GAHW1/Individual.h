#pragma once
#include "pch.h"

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

class Individual
{
public:
	int sumProfit = 0;
	int sumWeight = 0;
	float fitness = 0;
	int geneSize = 500;
	int genes[500];

	Individual();
	void evaluation(_itemSet[]);
};

#endif