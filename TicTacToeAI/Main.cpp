#include <iostream>
#include <vector>
#include <random>
#include <string>
using namespace std;

void generateStatistics();
bool checkWin(const string grid, const char winner);
void countOutcomes(const string grid, const char player, int& wins, int& losses);
string computeMove(const string grid, const char playerWhoMakesMove);
bool randomCheck();

const char FREE = ' ';
const char XS = 'X';
const char OS = 'O';

const string emptyGrid = string(9, FREE);

vector<string> statistics;

void generateStatistics()
{
	// Lets suppose FREE is a 0, XS is a 1 and OS is a 2. 
	// Then the grid cell is a ternary digit and it is't complicated 
	// to go through all game cases.
	string currentGrid = emptyGrid;
	bool overflow;
	do
	{
		int numberXs = 0, numberOs = 0;
		for (int i = 0; i < 9; i++)
		{
			if (currentGrid[i] == XS) numberXs++;
			else if (currentGrid[i] == OS) numberOs++;
		}

		bool isXWinner = checkWin(currentGrid, XS);
		bool isOWinner = checkWin(currentGrid, OS);

		if ((abs(numberXs - numberOs) == 0 || abs(numberXs - numberOs) == 1) &&
			(isXWinner && !isOWinner || !isXWinner && isOWinner) &&
			(isXWinner && numberXs >= numberOs || isOWinner && numberOs >= numberXs))
		{
			char firstMoveMake;
			if (isXWinner) 
				firstMoveMake = (numberXs == numberOs ? OS : XS);
			if (isOWinner) 
				firstMoveMake = (numberXs == numberOs ? XS : OS);

			char winner = (isXWinner ? XS : OS);

			statistics.push_back(currentGrid + firstMoveMake + winner);
		}

		overflow = false;
		for (int i = 8; i >= 0; i--)
		{
			if (currentGrid[i] == FREE)
			{
				currentGrid[i] = XS;
				break;
			}
			else if (currentGrid[i] == XS)
			{
				currentGrid[i] = OS;
				break;
			}
			else if (currentGrid[i] == OS)
			{
				if (i - 1 < 0)
				{
					overflow = true;
					break;
				}
				else
				{
					currentGrid[i] = FREE;
					continue;
				}
			}
		}

	} while (!overflow);
}

bool checkWin(const string grid, const char winner)
{
	const string& g = grid;
	
	return
		(g[0] == winner && g[1] == winner && g[2] == winner) ||
		(g[3] == winner && g[4] == winner && g[5] == winner) ||
		(g[6] == winner && g[7] == winner && g[8] == winner) ||
		(g[0] == winner && g[3] == winner && g[6] == winner) ||
		(g[1] == winner && g[4] == winner && g[7] == winner) ||
		(g[2] == winner && g[5] == winner && g[8] == winner) ||
		(g[0] == winner && g[4] == winner && g[8] == winner) ||
		(g[2] == winner && g[4] == winner && g[6] == winner);
}

string computeMove(const string grid, const char playerWhoMakesMove)
{
	int minLosses = -1;
	int maxWins = -1;
	string result = grid;

	for (int p = 0; p < 9; p++)
	{
		if (grid[p] == FREE)
		{
			string gridWithMoveInP = grid;
			gridWithMoveInP[p] = playerWhoMakesMove;
			
			int lossesForP = 0, winsForP = 0;
			countOutcomes(gridWithMoveInP, playerWhoMakesMove, winsForP, lossesForP);

			if (minLosses == -1 ||
				lossesForP < minLosses ||
				lossesForP == minLosses && winsForP > maxWins ||
				lossesForP == minLosses && winsForP == maxWins && randomCheck())
			{
				minLosses = lossesForP;
				maxWins = winsForP;
				result = gridWithMoveInP;
			}
		}
	}

	return result;
}

bool randomCheck()
{
	static uniform_int_distribution<> dist(0, 1);
	return dist(default_random_engine()) != 0;
}

void countOutcomes(const string grid, const char player, int& wins, int& losses)
{
	wins = losses = 0;
	const string& g = grid;

	for (const string outcome : statistics)
	{
		const string& o = outcome;

		if ((o[9] == g[9]) &&
			(g[0] == FREE || g[0] == o[0]) &&
			(g[1] == FREE || g[1] == o[1]) &&
			(g[2] == FREE || g[2] == o[2]) &&
			(g[3] == FREE || g[3] == o[3]) &&
			(g[4] == FREE || g[4] == o[4]) &&
			(g[5] == FREE || g[5] == o[5]) &&
			(g[6] == FREE || g[6] == o[6]) &&
			(g[7] == FREE || g[7] == o[7]) &&
			(g[8] == FREE || g[8] == o[8]))
		{
			if (o[10] != player)// outcome[10] indicate a winner.
			{
				losses++;
			}
			else
			{
				wins++;
			}
		}
	}
}

int main()
{
	generateStatistics();

	// Represent the game grid as a string of Xs and Os.
	// The "+ XS" at the end of the string
	// encodes who makes the first move.
	string grid = emptyGrid + XS;
	char whoseTurn = XS;
	for (int i = 0; i < 9; i++)
	{
		if (whoseTurn == OS)
		//if (whoseTurn == OS || whoseTurn == XS)
		{
			grid = computeMove(grid, whoseTurn);
		}
		else
		{
			int x, y;
			bool problemWithInput = false;
			do
			{
				cin >> x >> y;
				problemWithInput =
					x < 0 || x > 2 ||
					y < 0 || y > 2 ||
					grid[x + y * 3] != FREE;

			} while (problemWithInput);

			grid[x + y * 3] = whoseTurn;
		}

		const string& g = grid;
		cout << g[0] << g[1] << g[2] << endl
			<< g[3] << g[4] << g[5] << endl
			<< g[6] << g[7] << g[8] << endl
			<< "---\n";

		if (checkWin(grid, whoseTurn))
		{
			cout << whoseTurn << " win..\n";
			return 0;
		}

		whoseTurn = (whoseTurn == XS ? OS : XS);
	}

	cout << "It is draw..\n";
	return 0;
}