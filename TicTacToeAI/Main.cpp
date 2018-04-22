#include <iostream>
#include <vector>
#include <string>
using namespace std;

void generateStatistics();
bool checkWin(const string grid, const char winner);

const char FREE = '#';
const char XS = 'X';
const char OS = 'O';

// Represent the game grid as a string of Xs and Os.
string grid;
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

			statistics.push_back(currentGrid + winner + firstMoveMake);
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

int main()
{
	generateStatistics();
}