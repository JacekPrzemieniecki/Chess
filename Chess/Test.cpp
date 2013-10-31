#include "Test.h"
#include "Board.h"
#include "MoveGenerator.h"
#include <iostream>
#include <vector>

Test::Test()
{
}


Test::~Test()
{
}


int countPossibilities(Board* b, int depth)
{
	int poss = 0;
	vector<Move*> possibleMoves;
	GenerateAll(*b, possibleMoves);
	if (depth == 0)
	{
		int s = possibleMoves.size();
		for (vector<Move*>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); it++)
		{
			delete *it;
		}
		return s;
	}
	else
	{
		for (vector<Move*>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); it++)
		{
			Board* newBoard = new Board(b, **it);
			poss += countPossibilities(newBoard, depth - 1);
			delete newBoard;
			delete *it;
		}
	}
	return poss;
}

void Test::GeneratorTest()
{
	Board* b = new Board();
	for (int i = 0; i < 6; i++)
	{
		cout << "Depth: " << i << " Possibilities: " << countPossibilities(b, i) << endl;
	}
	delete b;
}
