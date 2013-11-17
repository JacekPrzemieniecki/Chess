#pragma once

#include "Board.h"
#include "Move.h"
#include <set>

class Game
{
public:
	Game();
	void TryMakeMove(Move& move);
	void Undo();
	PieceType GetPiece(int position);
	std::set<int>& GetWhitePieces();
	std::set<int>& GetBlackPieces();
	Move& GetLastMove();

	bool gameOver = false;
	Board board;
};
