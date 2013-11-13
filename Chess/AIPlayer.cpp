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

Move AIPlayer::MakeMove(Board& board)
{
	cout << "AI preparing to make move" << endl;
	vector<Move*> possibleMoves;
	GenerateAll(board, possibleMoves);
	cout << "AI generated " << possibleMoves.size() << " moves" << endl;
	Move chosenMove = *(possibleMoves[rand() % possibleMoves.size()]);
	cout << "Choosing move from: " << chosenMove.from << " to " << chosenMove.to << endl;
	int i = 0;
	Board* boardAfterMove = new Board(&board, chosenMove);
	cout << "Checking if king at position " << boardAfterMove->FindKing(false) << " is in check after AI move" << endl;
	while (IsAttacked(*boardAfterMove, boardAfterMove->FindKing(false), false))
	{
		cout << "King in check, generating another move" << endl;
		chosenMove = *(possibleMoves[rand() % possibleMoves.size()]);
		boardAfterMove = new Board(&board, chosenMove);
	}
	for (vector<Move*>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); it++)
	{
		delete *it;
	}
	return chosenMove;
}