#include "pch.h"
#include "Individual.h"

Individual::Individual()
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1);

	for (int i = 0; i < GENE_SIZE; i++)
	{
		genes[i] = dist(rng);
	}
}

void Individual::evaluation(_itemSet itemSet[])
{
	int _sumWeight = 0;
	int _sumProfit = 0;
	for (int i = 0; i < GENE_SIZE; i++)
	{
		if (genes[i])
		{
			_sumWeight += itemSet[i].weight;
			_sumProfit += itemSet[i].profit;
		}
	}
	
	if (_sumWeight < CAPACITY)
	{
		sumWeight = _sumWeight;
		sumProfit = _sumProfit;
	}
	else {
		sumWeight = 0;
		sumProfit = 0;
	}
}