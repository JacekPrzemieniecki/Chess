#include "Game.h"
#include "MoveGenerator.h"
#include "MoveValidator.h"
#include "AIPlayer.h"
#include <iostream>
#include <thread>

using namespace std;

Game::Game():
board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{
}

PieceType Game::GetPiece(int position)
{
	return board[position];
}

set<int>& Game::GetWhitePieces()
{
	return board.whitePieces;
}

set<int>& Game::GetBlackPieces()
{
	return board.blackPieces;
}

Move& Game::GetLastMove()
{
	return board.GetLastMove();
}

void Game::Undo()
{
	board.UndoMove();
}

void Game::TryMakeMove(Move& move)
{
	//cout << "Asked to move from: " << move.from << " to: " << move.to << endl;
	if (!ValidateMove(board, move))
	{
		//cout << "Move invalid";
		return;
	}
	if (!IsMoveLegal(board, move))
	{
		//cout << "Move leaves king in check";
		return;
	}
	//cout << "Move valid" << endl;
	board.MakeMove(move);
	if (!IsMovePossible(board))
	{
		cout << "Game Over" << endl;
		gameOver = true;
		return;
	}

	if (!board.whiteToMove)
	{
		//AIPlayer::MakeMove(*this);
		thread t(AIPlayer::MakeMove, ref(*this));
		t.detach();
	}
	
}