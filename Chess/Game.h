#pragma once

#include "Board.h"
#include "Move.h"
#include "AIPlayer.h"
#include <vector>

class Game
{
public:
	Game();
	~Game();
	void TryMakeMove(Move& move);
	PieceType GetPiece(int position);
	std::vector<int>& GetWhitePieces();
	std::vector<int>& GetBlackPieces();
	Move& GetLastMove();

	bool gameOver = false;
	Board* board;
private:
	AIPlayer aiPlayer;
};
