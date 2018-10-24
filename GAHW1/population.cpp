#include "pch.h"
#include "population.h"

Population::Population(int size)
{
	this->popSize = size;
}

void Population::Initialize()
{
	for (int i = 0; i < popSize; i++)
	{
		individual[i] = Individual();
	}
}

void Population::wheelSelection()
{
	Individual _tmpInd[POPULATION_SIZE];

	for (int i = 0; i < popSize; i++)
	{
		float fitSum = 0;
		float partialSum = 0;
		float pin = 0;

		int k = 0;

		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_real_distribution<> dist(0.0, 1.0);

		pin = dist(rng);

		for (int i = 0; i < POPULATION_SIZE; i++)
		{
			fitSum += individual[i].fitness;
		}

		partialSum = individual[i].fitness;

		while (k < POPULATION_SIZE - 1 && pin > partialSum / fitSum)
		{
			k++;
			partialSum += individual[k].fitness;
		}

		_tmpInd[i] = individual[k];
	}

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		individual[i] = _tmpInd[i];
	}
}

void Population::tournamentSelection()
{
	Individual _tmpInd[POPULATION_SIZE];

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		int k = 0;
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, POPULATION_SIZE - 1);
		k = dist(rng);

		if (individual[i].fitness < individual[k].fitness)
		{
			_tmpInd[i] = individual[k];
		}
		else {
			_tmpInd[i] = individual[i];
		}
	}
}

void Population::crossover()
{
	for (int i = 0; i < popSize / 2; i++)
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_real_distribution<> dist_real(0.0, 1.0);
		std::uniform_int_distribution<std::mt19937::result_type> dist_int(0, GENE_SIZE - 2);

		if (dist_real(rng) <= CROSSOVER_PROB)
		{
			int pos[3];
			for (int j = 0; j < 3; j++)
			{
				pos[j] = dist_int(rng);
			}
			std::sort(pos, pos + 3);

			for (int j = pos[0] + 1; j < pos[1]; j++)
			{
				int aux = individual[i].genes[j];
				individual[i].genes[j] = individual[i + popSize / 2].genes[j];
				individual[i + popSize / 2].genes[j] = aux;
			}
			for (int j = pos[2] + 1; j < GENE_SIZE; j++)
			{
				int aux = individual[i].genes[j];
				individual[i].genes[j] = individual[i + popSize / 2].genes[j];
				individual[i + popSize / 2].genes[j] = aux;
			}
		}
	}
}

void Population::mutation()
{
	for (int i = 0; i < popSize; i++)
	{
		for (int j = 0; j < GENE_SIZE; j++)
		{
			std::random_device rd;
			std::mt19937 rng(rd());
			std::uniform_real_distribution<> dist(0, 1);

			if (dist(rng) < MUTATION_PROB) {
				if (individual[i].genes[j] == 0) {
					individual[i].genes[j] = 1;
				}
				else {
					individual[i].genes[j] = 0;
				}
			}
		}
	}
}

std::string Population::evaluation(_itemSet itemSet[])
{
	float profitSet[POPULATION_SIZE];

	int _max = 0;
	int _min = INT_MAX;
	float _arrayMin = 0;

	float sum = 0;
	float means = 0;
	float _varSum = 0;
	float deviations = 0;

	float _profitSum = 0;
	float avgProfit = 0;

	std::string result;

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		individual[i].evaluation(itemSet);
		profitSet[i] = individual[i].sumProfit;

		_max = std::max(_max, individual[i].sumProfit);
		if (individual[i].sumProfit != 0) {
			_min = std::min(_min, individual[i].sumProfit);
		}
	}

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		if (profitSet[i] == 0) { continue; }
		profitSet[i] = (float)(profitSet[i] - _min) / (float)(_max - _min);
		sum += profitSet[i];
	}

	means = sum / POPULATION_SIZE;

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		_varSum += std::pow(profitSet[i] - means, 2);
	}

	deviations = std::sqrt(_varSum / POPULATION_SIZE);

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		profitSet[i] = (profitSet[i] - means) / deviations;
	}

	_arrayMin = *std::min_element(profitSet, profitSet + POPULATION_SIZE);

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		profitSet[i] += -(_arrayMin);
		individual[i].fitness = profitSet[i];
		_profitSum += individual[i].sumProfit;
	}

	avgProfit = _profitSum / POPULATION_SIZE;
	float key = *std::max_element(profitSet, profitSet + POPULATION_SIZE);
	for (int i = 0; i < POPULATION_SIZE; i++) {
		if (key == profitSet[i])
		{
			std::cout << "Avg Profit: " << avgProfit << std::endl;
			std::cout << "Best Fitness: " << individual[i].fitness << std::endl;
			std::cout << "Best Profit: " << individual[i].sumProfit << std::endl;
			result = std::to_string(avgProfit) +
				'\t' +
				std::to_string(individual[i].fitness) +
				'\t' +
				std::to_string(individual[i].sumProfit);
			break;
		}
	}

	return result;
}