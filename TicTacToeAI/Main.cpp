#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <string>
#include <algorithm>
using namespace std;

string computeMove(const string grid, const char playerWhoMakesMove);
int calculateMoveEstimate(const string grid, const char playerToMaximize, const bool isMaximizingPlayerMove);
bool checkWin(const string grid, const char winner);
bool checkNoMoreMovesLeft(const string grid);
bool randomCheck();

const char FREE = ' ';
const char XS = 'X';
const char OS = 'O';

const int WIN_ESTIMATE = +1;
const int LOOSE_ESTIMATE = -1;
const int DRAW_ESTIMATE = 0;

const string emptyGrid = string(9, FREE);

string computeMove(const string grid, const char playerWhoMakesMove)
{
	int bestMove;
	int bestEstimate;
	bool isBestEstimateSet = false;

	for (int move = 0; move < 9; move++)
	{
		if (grid[move] == FREE)
		{
			string gridWithMove = grid;
			gridWithMove[move] = playerWhoMakesMove;
			int estimateForMove = calculateMoveEstimate(gridWithMove, playerWhoMakesMove, false);

			if (!isBestEstimateSet || 
				estimateForMove > bestEstimate ||
				estimateForMove == bestEstimate && randomCheck())
			{
				bestEstimate = estimateForMove;
				bestMove = move;
			}
			
			isBestEstimateSet = true;
		}
	}

	string result = grid;
	result[bestMove] = playerWhoMakesMove;

	return result;
}

map<tuple<string, char, bool>, int> estimatesMemory;

int calculateMoveEstimate(const string grid, const char playerToMaximize, const bool isMaximizingPlayerMove)
{
	const char anotherPlayer = (playerToMaximize == XS ? OS : XS);
	const auto state = tuple<string, char, bool>(grid, playerToMaximize, isMaximizingPlayerMove);

	if (estimatesMemory.find(state) != estimatesMemory.end())
	{
		return estimatesMemory[state];
	}
	
	if (checkWin(grid, playerToMaximize))
	{
		estimatesMemory[state] = WIN_ESTIMATE;
		return WIN_ESTIMATE;
	}
	else if (checkWin(grid, anotherPlayer))
	{
		estimatesMemory[state] = LOOSE_ESTIMATE;
		return LOOSE_ESTIMATE;
	}
	else if (checkNoMoreMovesLeft(grid))
	{
		estimatesMemory[state] = DRAW_ESTIMATE;
		return DRAW_ESTIMATE;
	}
	else
	{
		int estimate;
		bool isEstimateSet = false;

		for (int move = 0; move < 9; move++)
		{
			if (grid[move] == FREE)
			{
				string gridWithMove = grid;
				gridWithMove[move] = (isMaximizingPlayerMove ? playerToMaximize : anotherPlayer);
				int estimateForMove = calculateMoveEstimate(gridWithMove, playerToMaximize, !isMaximizingPlayerMove);

				if (!isEstimateSet)
				{
					estimate = estimateForMove;
					isEstimateSet = true;
				}
				else
				{
					if (isMaximizingPlayerMove)
					{
						estimate = max(estimate, estimateForMove);
					}
					else
					{
						estimate = min(estimate, estimateForMove);
					}
				}

				if (isMaximizingPlayerMove && estimate == WIN_ESTIMATE ||
					!isMaximizingPlayerMove && estimate == LOOSE_ESTIMATE)
					break;
			}
		}

		estimatesMemory[state] = estimate;
		return estimate;
	}
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

bool checkNoMoreMovesLeft(const string grid)
{
	const string& g = grid;

	return
		g[0] != FREE && g[1] != FREE && g[2] != FREE &&
		g[3] != FREE && g[4] != FREE && g[5] != FREE &&
		g[6] != FREE && g[7] != FREE && g[8] != FREE;
}

bool randomCheck()
{
	static random_device rd;
	static mt19937 gen(rd());
	static uniform_int_distribution<> dist(0, 1);
	bool check = dist(gen) != 0;
	return check;
}

int main()
{
	string grid = emptyGrid;
	char whoseTurn = XS;
	for (int i = 0; i < 9; i++)
	{
		if (whoseTurn == OS)
		//	if (whoseTurn == OS || whoseTurn == XS)
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