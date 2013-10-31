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

vector<int>& Game::GetWhitePieces()
{
	return board->whitePieces;
}

vector<int>& Game::GetBlackPieces()
{
	return board->blackPieces;
}

Move& Game::GetLastMove()
{
	return board->lastMove;
}

void Game::TryMakeMove(Move& move)
{
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
	board = new Board(board, move);
	if (!IsMovePossible(*board))
	{
		gameOver = true;
		return;
	}
	Move aiMove = aiPlayer.MakeMove(*board);
	cout << "Ai moving from: " << aiMove.from << " to: " << aiMove.to << endl;
	board = new Board(board, aiMove);
	if (!IsMovePossible(*board))
	{
		gameOver = true;
		return;
	}
}