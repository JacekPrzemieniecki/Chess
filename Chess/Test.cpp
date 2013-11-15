#include "Test.h"
#include "Board.h"
#include "MoveGenerator.h"
#include <iostream>
#include <vector>
#include "MoveValidator.h"


int countPossibilities(Board& b, int depth)
{
	int poss = 0;
	vector<Move*> possibleMoves;
	possibleMoves.reserve(100);
	GenerateAll(b, possibleMoves);
	if (depth == 0)
	{
		for (Move* movePtr : possibleMoves)
		{
			if (IsMoveLegal(b, *movePtr))
			{
				poss++;
			}
			delete movePtr;
		}
	}
	else
	{
		for (Move* movePtr : possibleMoves)
		{
			if (IsMoveLegal(b, *movePtr))
			{
				b.MakeMove(*movePtr);
				poss += countPossibilities(b, depth - 1);
				b.UndoMove();
			}
			delete movePtr;
		}
	}
	return poss;
}

void Test::GeneratorTest(int depth)
{
	Board b;
	cout << "Testing Move generator" << endl;
	for (int i = 0; i < depth; i++)
	{
		cout << "Depth: " << i << " Possibilities: " << countPossibilities(b, i) << endl;
	}
}
