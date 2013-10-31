#include "AIPlayer.h"
#include "MoveGenerator.h"
#include "MoveValidator.h"
#include <vector>
#include <iostream>
#include <thread>

using namespace std;

AIPlayer::AIPlayer()
{
}


AIPlayer::~AIPlayer()
{
}

Move AIPlayer::MakeMove(Board& board)
{
	vector<Move*> possibleMoves;
	GenerateAll(board, possibleMoves);
	Move chosenMove = *(possibleMoves[rand() % possibleMoves.size()]);
	int i = 0;
	Board* boardAfterMove = new Board(&board, chosenMove);
	while (IsAttacked(*boardAfterMove, boardAfterMove->FindKing(false), false))
	{
		chosenMove = *(possibleMoves[rand() % possibleMoves.size()]);
		boardAfterMove = new Board(&board, chosenMove);
	}
	for (vector<Move*>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); it++)
	{
		delete *it;
	}
	return chosenMove;
}