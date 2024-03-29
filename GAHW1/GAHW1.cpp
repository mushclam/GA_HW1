/*
1) Roulette wheel GA
2) (pair-wise) tournament GA
Common: 3-point crossover, bit-wise mutation
*/

#include "pch.h"
#include "population.h"
#include "Individual.h"

std::string inputString;

_itemSet itemSet[500];
std::string rouletteResultSet[POPULATION_SIZE];
std::string tournamentResultSet[POPULATION_SIZE];

int main()
{
	int generation = 0;
	
	std::ifstream inFile("TestData(0-1Knapsack).txt");
	if (inFile.is_open())
	{
		int linenum = 0;
		while (!inFile.eof())
		{
			std::getline(inFile, inputString);
			std::string delimiter = "\t";

			if (!std::regex_match(inputString, std::regex("[0-9\t]*")))
			{
				continue;
			}
			
			size_t pos = 0;
			int colnum = 0;
			std::string token;
			while ((pos = inputString.find(delimiter)) != std::string::npos)
			{
				if (colnum == 0)
				{
					inputString.erase(0, pos + delimiter.length());
					colnum++;
					continue;
				}
				token = inputString.substr(0, pos);
				itemSet[linenum].weight = stoi(token);
				inputString.erase(0, pos + delimiter.length());
				colnum++;
			}
			itemSet[linenum].profit = stoi(inputString);
			linenum++;
		}
		inFile.close();
	}

	Population roulette(POPULATION_SIZE);
	Population tournament(POPULATION_SIZE);

	std::cout << "Roulette Generation (" << generation << "):" << std::endl;
	rouletteResultSet[generation] = roulette.evaluation(itemSet);
	std::cout << "Tournament Generation (" << generation << "):" << std::endl;
	tournamentResultSet[generation] = tournament.evaluation(itemSet);

	while (generation < END_GENERATION)
	{
		generation++;
		roulette.wheelSelection();
		roulette.crossover();
		roulette.mutation();

		std::cout << "Roulette Generation (" << generation << "):" << std::endl;
		rouletteResultSet[generation] = roulette.evaluation(itemSet);

		tournament.tournamentSelection();
		tournament.crossover();
		tournament.mutation();

		std::cout << "Tournament Generation (" << generation << "):" << std::endl;
		tournamentResultSet[generation] = tournament.evaluation(itemSet);
	}

	std::ofstream outFile("result.txt");
	if (outFile.is_open())
	{
		outFile << "Roulette Result\n\n";
		outFile << "Gen\tAvg\tBestFit\tBestProfit\n";
		for (int i = 0; i < POPULATION_SIZE; i++)
		{
			outFile << i << "\t" << rouletteResultSet[i] << std::endl;
		}

		outFile << "\n\nTournament Result\n\n";
		outFile << "Gen\tAvg\tBestFit\tBestProfit\n";
		for (int i = 0; i < POPULATION_SIZE; i++)
		{
			outFile << i << "\t" << tournamentResultSet[i] << std::endl;
		}
	}
	outFile.close();
}