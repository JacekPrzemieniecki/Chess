#include "Game.h"
#include "MoveGenerator.h"
#include "MoveValidator.h"
#include <iostream>

using namespace std;

Game::Game()
{
	board = new Board();
}

Game::~Game()
{
}

PieceType Game::GetPiece(int position)
{
	return (*board)[position];
}

set<int>& Game::GetWhitePieces()
{
	return board->whitePieces;
}

set<int>& Game::GetBlackPieces()
{
	return board->blackPieces;
}

Move& Game::GetLastMove()
{
	return board->lastMove;
}

void Game::TryMakeMove(Move& move)
{
	board->Print();
	cout << "Asked to move from: " << move.from << " to: " << move.to << endl;
	if (!ValidateMove(*board, move))
	{
		cout << "Move invalid";
		return;
	}
	if (!IsMoveLegal(*board, move, board->whiteToMove))
	{
		cout << "Move leaves king in check";
		return;
	}
	cout << "Move valid" << endl;
	board->MakeMove(move);
	board->Print();
	if (!IsMovePossible(*board))
	{
		cout << "Game Over" << endl;
		gameOver = true;
		return;
	}
	Move aiMove = aiPlayer.MakeMove(*board);
	cout << "Ai moving from: " << aiMove.from << " to: " << aiMove.to << endl;
	board->MakeMove(aiMove);
	board->Print();
	if (!IsMovePossible(*board))
	{
		cout << "Game Over" << endl;
		gameOver = true;
		return;
	}
}