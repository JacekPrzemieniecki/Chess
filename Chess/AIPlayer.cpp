#include "AIPlayer.h"
#include "MoveGenerator.h"
#include "MoveValidator.h"
#include <vector>
#include <iostream>

using namespace std;

AIPlayer::AIPlayer()
{
}


AIPlayer::~AIPlayer()
{
}

Move AIPlayer::MakeMove(Board board)
{
	cout << "AI preparing to make move" << endl;
	vector<Move*> possibleMoves;
	GenerateAll(board, possibleMoves);
	cout << "AI generated " << possibleMoves.size() << " moves" << endl;
	Move chosenMove = *(possibleMoves[rand() % possibleMoves.size()]);
	cout << "Choosing move from: " << chosenMove.from << " to " << chosenMove.to << endl;
	int i = 0;
	board.MakeMove(chosenMove);
	cout << "Checking if king at position " << board.FindKing(false) << " is in check after AI move" << endl;
	while (IsAttacked(board, board.FindKing(false), false))
	{
		board.UndoMove();
		cout << "King in check, generating another move" << endl;
		chosenMove = *(possibleMoves[rand() % possibleMoves.size()]);
		board.MakeMove(chosenMove);
	}
	board.UndoMove();
	for (vector<Move*>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); it++)
	{
		delete *it;
	}
	return chosenMove;
}